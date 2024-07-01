#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "Graph.hpp"
#include "kosaraju.hpp"
#include "HandleClients.hpp"
#include <string.h>

#define GRAPH_IMPL AdjacencyGraph
// #define GRAPH_IMPL ListGraph

// Convert the GRAPH_IMPL constant to a string literal
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include TOSTRING(GRAPH_IMPL.hpp)

#define PORT 9034
#define MAX_USERS 5
#define POLL_TIMEOUT 5

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

using std::cin, std::cout, std::set, std::string;

bool handle_user_input(int fd, Graph **g, std::string input)
{
    int pipefd[2];
    pipe(pipefd);
    dup2(pipefd[PIPE_READ_END], STDIN_FILENO);
    dup2(pipefd[PIPE_WRITE_END], STDOUT_FILENO);
    
    if (!g)
    {
        throw std::invalid_argument("Graph pointer is NULL");
    }
    std::istringstream is(input);
    std::string command;
    std::getline(is, command, ' ');
    if (command == "Kosaraju")
    {
        if (!*g)
        {
            cout << "Please create a graph using Newgraph <n>,<m> first" << std::endl;
            return false;
        }
        auto comps = kosaraju(**g);
        cout << "The strongly connected components are: " << std::endl;
        for (auto comp : comps)
        {
            for (vertex v : comp)
            {
                std::cout << v << " ";
            }
            std::cout << std::endl;
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
            cout << "Not enough parameters detected, command ignored" << std::endl;
        }
        size_t n = strtoull(param1.c_str(), nullptr, 10), m = strtoull(param2.c_str(), nullptr, 10);
        if (*g)
            delete *g;
        std::vector<std::pair<vertex, vertex>> edges;
        for (size_t i = 0; i < m; ++i) // FIXME
        {
            vertex src, dst;
            cin >> src >> dst;
            edges.push_back(std::make_pair(src, dst));
        }
        *g = new GRAPH_IMPL(n, edges);
        return false;
    }
    else if (command == "Newedge")
    {
        if (!*g)
        {
            cout << "Please create a graph using Newgraph <n>,<m> first" << std::endl;
            return false;
        }
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            cout << "Not enough parameters detected, command ignored" << std::endl;
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(*g)->add_edge(src, dst))
            cout << "Edge already exists" << std::endl;
        else
            cout << "Edge was created successfuly" << std::endl;
        return false;
    }
    else if (command == "Removeedge")
    {
        if (!*g)
        {
            cout << "Please create a graph using Newgraph <n>,<m> first" << std::endl;
            return false;
        }
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            cout << "Not enough parameters detected, command ignored" << std::endl;
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(*g)->remove_edge(src, dst))
            cout << "Edge does not exist" << std::endl;
        return false;
    }
    else if (command == "Exit")
    {
        return true;
    }
    else
    {
        cout << "Unkown command" << std::endl;
        return false;
    }
}

int main()
{

    Graph *g = nullptr;

    int server_fd = -1;
    int new_socket = -1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_SEGMENT_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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

    std::vector<struct pollfd> pfds;

    // Listen for incoming connections
    if (listen(server_fd, MAX_USERS) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (new_socket > 0)
    {
        pfds.push_back((struct pollfd){
            .fd = new_socket,
            .events = POLLIN,
            .revents = 0,
        });
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Set the socket to non-blocking mode
    if (fcntl(server_fd, F_GETFL | O_NONBLOCK, 0) < 0)
    {
        perror("fcntl(F_GETFL | O_NONBLOCK)");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    GraphInput input_params = initialize_graph(pfds.back().fd);

    Graph *g = new GRAPH_IMPL(input_params.vertex_count, input_params.edges);

    bool to_exit = false;
    while (!to_exit)
    {
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        if (new_socket > 0)
        {
            pfds.push_back((struct pollfd){
                .fd = new_socket,
                .events = POLLIN,
                .revents = 0,
            });
        }
        int polled = poll(pfds.data(), pfds.size(), POLL_TIMEOUT);
        if (polled > 0)
        {
            for (auto it = pfds.begin(); it != pfds.end(); ++it)
            {
                if (it->revents & POLLIN)
                {
                    string input = handle_client_input(it->fd); // will check what is the input and send the apropriate output.
                    handle_user_input(&g, input);
                }
            }
        }
    }

    for (struct pollfd pfd : pfds)
    {
        close(pfd.fd);
    }
    close(server_fd);
}
