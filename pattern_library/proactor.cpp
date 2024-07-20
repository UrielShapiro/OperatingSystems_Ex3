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
        std::lock_guard<std::mutex> running_guard(this->running_mutex); // Lock the running mutex, will be unlocked when the scope ends
        this->running = true;
    }
    main_thread = new std::thread(&Proactor::proactor_main, this);
}

void Proactor::stop()
{
    if (!main_thread)   // If the thread is not running, exit
        return;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);   // Lock the running mutex, will be unlocked when the scope ends
        running = false;        
    }
    main_thread->join();    // Join the thread, will wait for the thread to finish
    delete main_thread;
    main_thread = nullptr;
}

void Proactor::proactor_main()
{
    bool still_running;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex);
        still_running = running;    // Update the still_running variable
    }

    // Proactor main loop
    while (still_running)
    {
        fd_set set;
        FD_ZERO(&set);  // Zero the set
        FD_SET(sockfd, &set);   // Add the socketfd to the set
        struct timeval timeout
        {
            0, ACCEPT_TO_MS * 1000
        };
        int select_result = select(sockfd + 1, &set, NULL, NULL, &timeout); // Wait for a connection, with a timeout. Blocking call for the timeout.
        // Is set for a timeout, to check if the proactor should stop
        if (select_result > 0)  // If there is a connection
        {
            int client_fd = accept(sockfd, NULL, NULL);
            std::thread(client_handler, client_fd).detach();    // Create a new thread the client_handler with the client_fd. Detach the thread.
        }
        else if (select_result < 0)
        {
            throw std::runtime_error(strerror(errno));
        }
        {
            std::lock_guard<std::mutex> running_guard(running_mutex);
            still_running = running;    // Update the still_running variable
        }
    }
}

std::thread& Proactor::get_thread()
{
    return *(this->main_thread);
}
