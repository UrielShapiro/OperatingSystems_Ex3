#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <set>
#include <string>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include "../graph_library/Graph.hpp"
#include "../graph_library/kosaraju.hpp"
#include "../pattern_library/proactor.hpp"

#define GRAPH_IMPL AdjacencyGraph
#include "../graph_library/AdjacencyGraph.hpp"

#define PORT 9034
#define MAX_USERS 5
#define MAX_SEGMENT_SIZE 65535

#define DEBUG

using std::cin, std::cout, std::endl, std::set, std::string;

Graph *g = nullptr;
std::shared_mutex graph_mutex;

/**
 * @brief Send a message to the client
 * @return true if an error occurred, false otherwise
 */
bool send_message(int fd, string message)
{
    return write(fd, message.c_str(), message.length() + 1) < 0;
}

/**
 * @brief Receive a message from the client
 * @return The received message
 * @throw Throws runtime_error if an error occurred
 */
string receive_message(int fd)
{
    char buffer[MAX_SEGMENT_SIZE];
    ssize_t bytes_read = read(fd, buffer, MAX_SEGMENT_SIZE);
    if (bytes_read < 0)
    {
        throw std::runtime_error("Something went wrong when trying to read from the socket");
    }
    if (bytes_read == 0)
    {
        throw std::runtime_error("Connection closed by the client");
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer if it's a string
    return buffer;
}

/**
 * @brief Handle the user input
 * @return true if the client requested to close the connection, false otherwise
 */
bool handle_user_input(int fd, string input)
{
    std::istringstream is(input);
    std::string command;
    std::getline(is, command, ' ');
    if (command == "Kosaraju")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
        }
        std::set<std::set<vertex>> comps;
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            comps = kosaraju(*g);
        }
        string message;
        message += "The strongly connected components are: \n";
        for (auto comp : comps)
        {
            for (vertex v : comp)
            {
                message += std::to_string(v) + ' ';
            }
            message += '\n';
        }
        if (send_message(fd, message))
        {
            throw std::runtime_error("Error sending a message to the client");
        }
        return false;
    }
    else if (command == "Newgraph")
    {
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        size_t n = strtoull(param1.c_str(), nullptr, 10), m = strtoull(param2.c_str(), nullptr, 10);
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);

            if (g)
                delete g;

            std::vector<std::pair<vertex, vertex>> edges;
            for (size_t i = 0; i < m; ++i)
            {
                vertex src, dst;
                string received_edge = receive_message(fd);
                cout << "Received edge: " << received_edge << std::endl;
                char *space;
                src = strtoull(received_edge.c_str(), &space, 10);
                dst = strtoull(space + 1, nullptr, 10);
                cout << "Parsed edge: " << src << " " << dst << std::endl;
                edges.push_back(std::make_pair(src, dst));
            }
            g = new GRAPH_IMPL(n, edges);
        }
        return false;
    }
    else if (command == "Newedge")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
        }
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g->add_edge(src, dst))
            {
                if (send_message(fd, "Edge already exists\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
            }
            else
            {
                if (send_message(fd, "Edge was created successfuly\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
            }
        }
        return false;
    }
    else if (command == "Removeedge")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
        }
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g->remove_edge(src, dst))
            {
                if (send_message(fd, "Edge does not exist\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
            }
            else
            {
                if (send_message(fd, "Edge was removed successfuly\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
            }
        }
        return false;
    }
    else if (command == "Exit")
    {
        std::cout << "Connection closed by the client" << std::endl;
        return true;
    }
    else
    {
        if (send_message(fd, "Unknown command\n"))
        {
            throw std::runtime_error("Error sending a message to the client");
        }
        return false;
    }
}

void server_main(int fd)
{
    bool run = true;

    std::function<void(int)> error_handler = [&](int fd)
    {
        close(fd);
        run = false;
    };

    while (run)
    {
        string input;
        if (send_message(fd, "Enter command: "))
        {
            std::cerr << ("Error sending a message to the client");
            error_handler(fd);
        }
        try
        {
            input = receive_message(fd);
        }
        catch (const std::runtime_error &e)
        {
            cout << e.what() << std::endl;
            error_handler(fd);
        }
        input.pop_back(); // Remove the newline character
        if (handle_user_input(fd, input))
        {
            error_handler(fd);
        }
    }
}

int main()
{
    Proactor proactor;
    int server_fd = -1;
    struct sockaddr_in address;
    int opt = 1;

    std::cout << "Waiting for incoming connections..." << std::endl;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_USERS) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    proactor.start(server_fd, server_main);
    while (proactor.get_running())
        ;
    proactor.stop();

    close(server_fd);
    return 0;
}
