#include <iostream>
#include <unistd.h>

#include "reactor.hpp"

void echo_to_fd_continuously(int fd)
{
    char c;
    read(fd, &c, sizeof(c));
    std::cout << "Read \"" << c << '\"';
}

int main()
{
    Reactor reactor;
    reactor.add_fd(STDIN_FILENO, echo_to_fd_continuously);
    reactor.start();
    sleep(10);
    reactor.stop();
}
