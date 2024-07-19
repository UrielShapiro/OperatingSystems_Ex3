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

#include TOSTRING(../graph_library/GRAPH_IMPL.hpp)

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
