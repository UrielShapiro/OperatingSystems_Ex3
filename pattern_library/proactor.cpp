#include "proactor.hpp"

#include <thread>
#include <vector>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

Proactor::Proactor() : running(false), main_thread(nullptr) {}

void Proactor::start(int sockfd, Handler client_handler)
{
    this->sockfd = sockfd;
    this->client_handler = client_handler;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        running = true;
    }
    if (main_thread)
        delete main_thread;
    main_thread = new std::thread(&Proactor::proactor_main, this);
}

void Proactor::stop()
{
    if (!main_thread)
        return;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        running = false;
    }
    main_thread->join();
    delete main_thread;
}

void Proactor::proactor_main()
{
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        throw std::runtime_error("Error setting socket in non-blocking mode");
    }

    bool still_running;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        still_running = running;
    }
    while (still_running)
    {
        int client_fd = accept(sockfd, nullptr, nullptr);
        worker_threads.push_back(std::thread(client_handler));
        {
            std::lock_guard<std::mutex> running_guard(running_mutex);
            still_running = running;
        }
    }
}
