#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

constexpr int ACCEPT_TO_MS = 10;

class Proactor
{
public:
    /**
     * The type used as a handler for accepted connections.
     * The function receives as an argument the file descriptor of the connected client.
     */
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
    /**
     * Creates a new proactor, intially not running, you can start it with start().
     */
    Proactor();
    /**
     * Destroys the proactor, calls stop().
     */
    ~Proactor();
    /**
     * Starts the proactor listening to sockfd, when a new client connects client_handler will be called with its socket.
     * sockfd should already be bound to a listening passive socket.
     * If the proactor is already running it will be stopped first.
     */
    void start(int sockfd, Handler client_handler);
    /**
     * Stops the proactor, does not stop client handling threads.
     * Could take up to ACCEPT_TO_MS ms for the proactor to stop.
     */
    void stop();
};
