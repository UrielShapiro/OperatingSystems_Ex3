#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>

#include "reactor.hpp"

Reactor::Reactor() : running(false), thread(nullptr), fds_count(0) {}

Reactor::~Reactor()
{
    this->stop();
}

bool Reactor::add_fd(int fd, Handler handler)
{
    this->vectors_mutex.lock();
    if (pfds.end() != std::find_if(pfds.begin(), pfds.end(), [fd](struct pollfd pfd)
                                   { return pfd.fd == fd; }))
    {
        return false;
    }
    this->pfds.push_back((struct pollfd){
        .fd = fd,
        .events = POLLIN,
        .revents = 0});
    this->handlers.push_back(handler);
    this->fds_count += 1;
    this->vectors_mutex.unlock();
    return true;
}

bool Reactor::remove_fd(int fd)
{
    this->vectors_mutex.lock();
    for (size_t i = 0; i < fds_count; ++i)
    {
        if (pfds[i].fd == fd)
        {
            pfds.erase(pfds.begin() + i);
            handlers.erase(handlers.begin() + i);
            fds_count -= 1;
            this->vectors_mutex.unlock();
            return true;
        }
    }
    this->vectors_mutex.unlock();
    return false;
}

void Reactor::start()
{
    running = true;
    thread = new std::thread(&Reactor::reactor_main, this);
}

void Reactor::stop()
{
    if (!thread) return;
    running_mutex.lock();
    running = false;
    running_mutex.unlock();
    thread->join();
    delete thread;
}

void Reactor::reactor_main()
{
    while ((this->running_mutex.lock(), this->running))
    {
        this->running_mutex.unlock();
        this->vectors_mutex.lock();
        poll(pfds.data(), pfds.size(), 0);
        for (size_t i = 0; i < this->fds_count; ++i)
        {
            if (pfds[i].revents & POLLIN)
            {
                handlers[i](pfds[i].fd);
            }
        }
        this->vectors_mutex.unlock();
    }
    this->running_mutex.unlock();
}
