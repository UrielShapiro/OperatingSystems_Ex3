#include "proactor.hpp"

#include <thread>
#include <vector>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

Proactor::Proactor() : running(false), main_thread(nullptr) {}

void Proactor::start(int sockfd, Handler client_handler)
{
    this->sockfd = sockfd;
    running_mutex.lock();
    running = true;
    running_mutex.unlock();
    this->client_handler = client_handler;
    main_thread = new std::thread(&Proactor::proactor_main, this);
}

void Proactor::stop()
{
    if (!main_thread)
        return;
    running_mutex.lock();
    running = false;
    running_mutex.unlock();
    main_thread->join();
    delete main_thread;
}

void Proactor::proactor_main()
{
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        throw std::runtime_error("Error setting socket in non-blocking mode");
    }
    while ((running_mutex.lock(), running))
    {
        int client_fd = accept(sockfd, nullptr, nullptr);
        pfds_mutex.lock();
        pfds.push_back(pollfd{
            .fd = client_fd,
            .events = POLLIN,
            .revents = 0
        });

        running_mutex.unlock();
        pfds_mutex.lock();
        poll(pfds.data(), pfds.size(), 0);
        for (size_t i = 0; i < pfds.size(); ++i)
        {
            if (pfds[i].revents & POLLIN)
            {
                worker_threads.push_back(std::thread(client_handler));
            }
        }
        pfds_mutex.unlock();
    }
    running_mutex.unlock();
}
