<<<<<<< HEAD
#pragma once

=======
>>>>>>> e2c1457 (Renamed Q5 to pattern_library)
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

<<<<<<< HEAD
/**
 * This class implements the reactor design pattern.
 * The reactor saves a list of file descriptors and handlers.
 * While running the reactor continuously polls the file descriptors for events.
 * When an event occurs on one of them (currently only event is information available for reading) the handler is called.
 */
class Reactor
{
public:
    /**
     * The type used as a handler for reactions to file descriptor events.
     * The function receives as an argument the file descriptor on which the event occured.
     */
=======
class Reactor
{
>>>>>>> e2c1457 (Renamed Q5 to pattern_library)
    using Handler = std::function<void (int)>;
private:
    bool running;
    std::mutex running_mutex;
    std::thread *thread;
    size_t fds_count;
    std::vector<Handler> handlers;
    std::vector<struct pollfd> pfds;
    std::mutex vectors_mutex;
    void reactor_main();
public:
<<<<<<< HEAD
    /**
     * Creates a new reactor, intially not running, you can add file descriptors to it and use start() to start it.
     */
    Reactor();
    /**
     * Destroys the reactor, calls stop().
     */
    ~Reactor();
    /**
     * Starts the reactor in a separate thread.
     */
    void start();
    /**
     * Stops the reactor, blocks until all currently polled operations are finished.
     */
    void stop();
    /**
     * Adds a new file descriptor to the reactor, when events occur on it handler will be called.
     */
    bool add_fd(int fd, Handler handler);
    /**
     * Removes from the reactor the file descriptor and its handler.
     */
=======
    Reactor();
    ~Reactor();
    void start();
    void stop();
    bool add_fd(int fd, Handler handler);
>>>>>>> e2c1457 (Renamed Q5 to pattern_library)
    bool remove_fd(int fd);
};
