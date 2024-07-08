#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

class Proactor
{
    using Handler = std::function<void(int)>;

private:
    Handler client_handler;
    int sockfd;
    bool running;
    std::mutex running_mutex;
    std::thread *main_thread;
    std::vector<std::thread> worker_threads;
    std::vector<struct pollfd> pfds;
    void proactor_main();

public:
    Proactor();
    ~Proactor();
    void start(int sockfd, Handler client_handler);
    void stop();
};
