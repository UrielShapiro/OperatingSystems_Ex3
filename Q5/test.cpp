#include <iostream>
#include <unistd.h>

#include "reactor.hpp"

void print_from_fd(int fd)
{
    char c;
    read(fd, &c, sizeof(c));
    std::cout << "Read " << c;
}

int main()
{
    Reactor reactor;
    reactor.add_fd(STDIN_FILENO, print_from_fd);
    reactor.start();
    sleep(10);
    reactor.stop();
}
