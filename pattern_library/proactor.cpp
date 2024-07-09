#include "proactor.hpp"

#include <thread>
#include <string.h>
#include <vector>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

Proactor::Proactor() : running(false), main_thread(nullptr) {}

Proactor::~Proactor()
{
    this->stop();
}

void Proactor::start(int sockfd, Handler client_handler)
{
    stop();
    this->sockfd = sockfd;
    this->client_handler = client_handler;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        running = true;
    }
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
    main_thread = nullptr;
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
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        struct timeval timeout
        {
            0, ACCEPT_TO_MS * 1000
        };
        int select_result = select(sockfd + 1, &set, NULL, NULL, &timeout);
        if (select_result > 0)
        {
            int client_fd = accept(sockfd, NULL, NULL);
            std::thread(client_handler, client_fd).detach();
        }
        else if (select_result < 0)
        {
            throw std::runtime_error(strerror(errno));
        }
        {
            std::lock_guard<std::mutex> running_guard(running_mutex);
            still_running = running;
        }
    }
}
