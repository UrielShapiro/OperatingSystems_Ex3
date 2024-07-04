#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

class Reactor
{
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
    Reactor();
    ~Reactor();
    void start();
    void stop();
    bool add_fd(int fd, Handler handler);
    bool remove_fd(int fd);
};
