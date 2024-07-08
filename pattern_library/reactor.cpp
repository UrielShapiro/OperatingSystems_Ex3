#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>

#include "reactor.hpp"

#define DEBUG

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
#ifdef DEBUG
    std::cout << "Removing fd\n";
    std::cout << "fds_count: " << fds_count << std::endl;
    std::cout << "pfds.size: " << pfds.size() << std::endl;
#endif
    // this->vectors_mutex.lock();  // Problematic line
    for (size_t i = 0; i < fds_count; ++i)
    {
#ifdef DEBUG
        std::cout << "Checking fd: " << pfds[i].fd << " VS " << fd << std::endl;
#endif
        if (pfds[i].fd == fd)
        {
#ifdef DEBUG
            std::cout << "Found!\n";
#endif
            pfds.erase(pfds.begin() + i);
            handlers.erase(handlers.begin() + i);
            fds_count -= 1;
            close(fd); // Uriel's addition - Not working
            this->vectors_mutex.unlock();
            return true;
        }
    }
    this->vectors_mutex.unlock();
    return false;
}

void Reactor::start()
{
    running_mutex.lock();
    running = true;
    running_mutex.unlock();
    thread = new std::thread(&Reactor::reactor_main, this);
}

void Reactor::stop()
{
    if (!thread)
        return;
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
            else if (pfds[i].revents & POLLNVAL)
            {
                this->remove_fd(pfds[i].fd);
            }
        }
        this->vectors_mutex.unlock();
    }
    this->running_mutex.unlock();
}
