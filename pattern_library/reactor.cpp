#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <variant>

#include "reactor.hpp"

#define DEBUG

Reactor::Reactor() : running(false), thread(nullptr), fds_count(0) {}

Reactor::~Reactor()
{
    this->stop();
}

bool Reactor::add_fd(int fd, Handler handler)
{
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
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
    return true;
}

bool Reactor::remove_fd(int fd)
{
#ifdef DEBUG
    std::cout << "Removing fd\n";
    std::cout << "fds_count: " << fds_count << std::endl;
    std::cout << "pfds.size: " << pfds.size() << std::endl;
#endif
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
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
            return true;
        }
    }
    return false;
}

void Reactor::start()
{
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        running = true;
    }
    thread = new std::thread(&Reactor::reactor_main, this);
}

void Reactor::stop()
{
    if (!thread)
        return;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        running = false;
    }
    thread->join();
    delete thread;
    thread = nullptr;
}

struct handler_action
{
    Reactor::Handler handler;
    int param;
    void operator()()
    {
        handler(param);
    }
};

struct remove_action
{
    Reactor *reactor;
    int param;
    void operator()()
    {
        reactor->remove_fd(param);
    }
};

void Reactor::reactor_main()
{
    bool still_running;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        still_running = running;
    }
    while (still_running)
    {
        std::vector<std::variant<handler_action, remove_action>> actions;
        {
            std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
            poll(pfds.data(), pfds.size(), 0);
            for (size_t i = 0; i < this->fds_count; ++i)
            {
                if (pfds[i].revents & POLLIN)
                {
                    actions.push_back(handler_action{handlers[i], pfds[i].fd});
                }
                else if (pfds[i].revents & POLLNVAL)
                {
                    actions.push_back(remove_action{this, pfds[i].fd});
                }
            }
        }

        for (auto &action : actions)
            std::visit([](auto &arg)
                       { arg(); }, action);

        {
            std::lock_guard<std::mutex> running_guard(running_mutex);
            still_running = running;
        }
    }
}
