#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "proactor.hpp"

void echo_to_fd_continuously(int fd)
{
    char c;
    while (read(fd, &c, sizeof(c)) > 0)
    {
        write(fd, &c, sizeof(c));
        std::cout << "Read \"" << c << '\"' << std::endl;
    }
    std::cout << "Client disconnected" << std::endl;
}


// most of this test code was generated using GitHib Copilot
int main()
{
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Bind the socket to a specific address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(5000);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Failed to listen for connections." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port 5000..." << std::endl;

    Proactor proactor;
    proactor.start(serverSocket, echo_to_fd_continuously);
    sleep(300);
    proactor.stop();

    close(serverSocket);

    return 0;
}