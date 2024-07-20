#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include "../graph_library/kosaraju.hpp"
#include "../graph_library/Graph.hpp"

#define GRAPH_IMPL AdjacencyGraph
// #define GRAPH_IMPL ListGraph

// Convert the GRAPH_IMPL constant to a string literal
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include TOSTRING(../graph_library/GRAPH_IMPL.hpp)  // Include the graph implementation

using std::cin, std::cout, std::set;

/**
 * @brief Handle user input
 * @param g The graph to operate on
 * @param input The user input
 * @return True if the program should exit, false otherwise
 */
bool handle_user_input(Graph **g, std::string input)
{
    if (!g) // Check if the graph pointer is pointing to a valid memory location
    {
        throw std::invalid_argument("Graph pointer is NULL");
    }

    std::istringstream is(input);   // Create a input stream from the input
    std::string command;
    std::getline(is, command, ' '); // Get the command from the input, separated by a space

    if (command == "Kosaraju")
    {
        if (!*g)    // Check if the graph has been created
        {
            cout << "Please create a graph using Newgraph <n>,<m> first" << std::endl;
            return false;
        }
        auto comps = kosaraju(**g);
        cout << "The strongly connected components are: " << std::endl;
        // Iterate over the strongly connected components and print them
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
        std::getline(is, param1, ',');  // Get the first parameter, separated by a comma
        std::getline(is, param2);       // Get the second parameter
        if (param1.length() == 0 || param2.length() == 0)   // Check that the parameters are not empty
        {
            cout << "Not enough parameters detected, command ignored" << std::endl;
        }
        // Convert the parameters to unsigned long long integers
        size_t n = strtoull(param1.c_str(), nullptr, 10);   // Number of vertices
        size_t m = strtoull(param2.c_str(), nullptr, 10);   // Number of edges

        if (*g)
            delete *g;
        std::vector<std::pair<vertex, vertex>> edges;   // Create a vector of pairs of vertices to store the edges
        for (size_t i = 0; i < m; ++i)
        {
            vertex src, dst;
            cin >> src >> dst;  // Get the source and destination vertices (separator by a space)
            edges.push_back(std::make_pair(src, dst));
        }
        *g = new GRAPH_IMPL(n, edges);  // Create a new graph with the specified number of vertices and edges
        return false;
    }
    else if (command == "Newedge")
    {
        if (!*g)    // Check if the graph has been created
        {
            cout << "Please create a graph using Newgraph <n>,<m> first" << std::endl;
            return false;
        }
        std::string param1, param2;
        std::getline(is, param1, ',');  // Get the first parameter, separated by a comma
        std::getline(is, param2);       // Get the second parameter
        if (param1.length() == 0 || param2.length() == 0)
        {
            cout << "Not enough parameters detected, command ignored" << std::endl;
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10);
        vertex dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(*g)->add_edge(src, dst))  // If the edge already exists, print that it already exists
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
        vertex src = strtoull(param1.c_str(), nullptr, 10);
        vertex dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(*g)->remove_edge(src, dst))               // If the edge does not exist, print that it does not exist
            cout << "Edge does not exist" << std::endl;
        else
            cout << "Edge was removed successfuly" << std::endl;
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
    std::string input;

    Graph *g = nullptr;
    bool exit = false;
    while (!exit)
    {
        cout << "Enter command: ";
        std::getline(cin, input);
        exit = handle_user_input(&g, input);
    }
    delete g;
    return 0;
}
