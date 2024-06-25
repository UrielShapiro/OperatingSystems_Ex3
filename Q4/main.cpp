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
#include "Graph.hpp"
#include "kosaraju.hpp"

#define GRAPH_IMPL AdjacencyGraph
// #define GRAPH_IMPL ListGraph

// Convert the GRAPH_IMPL constant to a string literal
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include TOSTRING(GRAPH_IMPL.hpp)

#define PORT 9034

using std::cin, std::cout, std::set;

bool handle_user_input(Graph **g, std::string input)
{
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
            cout<<"Edge was created successfuly"<<std::endl;
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
    int server_fd = -1
    int new_socket = -1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connection accepted" << std::endl;

    int valread;
    while ((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        std::cout << "Received: " << buffer << std::endl;
        send(new_socket, buffer, valread, 0);
        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
    }

    if (valread < 0) {
        perror("read");
    }

    close(new_socket);
    close(server_fd);


    std::string input;

    Graph *g = nullptr;
    bool exit = false;
    while (!exit)
    {
        cout << "Enter command: ";
        std::getline(cin, input);
        exit = handle_user_input(&g, input);
    }

    /*
    size_t vertex_count, edge_count;
    std::vector<std::pair<vertex, vertex>> edges;
    cout << "Enter number of vertices: ";
    cin >> vertex_count;
    cout << "Enter number of edges: ";
    cin >> edge_count;
    cout << "Enter edge pairs: ";
    for (size_t i = 0; i < edge_count; ++i)
    {
        vertex src, dst;
        cin >> src >> dst;
        edges.push_back(std::make_pair(src, dst));
    }

    Graph *g = new GRAPH_IMPL(vertex_count, edges);


    delete g;
    */
}