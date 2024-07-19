#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

// Accept timeout in milliseconds
constexpr int ACCEPT_TO_MS = 10;

/**
 * This class implements the proactor design pattern.
 * The proactor listens to a passive socket and when a new client connects it calls a handler with the client's socket.
 * The proactor uses a main thread to accept new connections and worker threads to handle the clients.
 * The proactor is started with start() and stopped with stop().
 * The handler is called with the client's socket as an argument.
 * The proactor is not thread safe, do not call start() or stop() from multiple threads.
 * The handler is called from a worker thread, make sure it is thread safe.
 */
class Proactor
{
public:
    /**
     * The type used as a handler for accepted connections.
     * The function receives as an argument the file descriptor of the connected client.
     */
    using Handler = std::function<void(int)>;

private:
    Handler client_handler; // The handler for accepted connections
    int sockfd;            // The socket file descriptor to listen to
    bool running;           // Whether the proactor is running
    std::mutex running_mutex;   // Mutex for the running variable
    std::thread *main_thread;   // The main thread of the proactor
    void proactor_main();                   // The main function of the proactor        

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

    /**
     * @returns The main thread of the proactor.
     */
    std::thread& get_thread();
};
