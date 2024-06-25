#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include "Graph.hpp"

#define GRAPH_IMPL AdjacencyGraph
// #define GRAPH_IMPL ListGraph

// Convert the GRAPH_IMPL constant to a string literal
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include TOSTRING(GRAPH_IMPL.hpp)

using std::cin, std::cout, std::set;

void visit(const Graph &g, std::map<vertex, bool> &visited, std::list<vertex> &l, vertex u)
{
    if (visited[u])
        return;
    visited[u] = true;
    for (auto v : g.get_neighbors(u))
        visit(g, visited, l, v);
    l.push_front(u);
}

void assign(const Graph &g, std::map<vertex, std::set<vertex>> &assignments, std::map<vertex, bool> &assigned, vertex u, vertex root)
{
    if (assigned[u])
        return;
    assignments[root].insert(u);
    assigned[u] = true;
    size_t n = g.get_vertex_count();
    for (vertex v = 1; v <= n; ++v)
    {
        auto v_neighbors = g.get_neighbors(v);
        if (v == u || std::find(v_neighbors.begin(), v_neighbors.end(), u) == v_neighbors.end())
            continue;
        assign(g, assignments, assigned, v, root);
    }
}

std::set<std::set<vertex>> kosaraju(const Graph &g)
{
    size_t n = g.get_vertex_count();
    std::map<vertex, bool> visited;
    std::list<vertex> l;
    for (vertex i = 1; i <= n; ++i)
        visited[i] = false;
    for (vertex u = 1; u <= n; ++u)
        visit(g, visited, l, u);
    std::map<vertex, std::set<vertex>> assignments;
    std::map<vertex, bool> assigned;
    for (vertex i = 1; i <= n; ++i)
        assigned[i] = false;
    for (auto u : l)
    {
        assign(g, assignments, assigned, u, u);
    }
    std::set<std::set<vertex>> result;
    for (auto &[_, comp] : assignments)
    {
        result.insert(comp);
    }
    return result;
}

void handle_user_input(Graph **g, std::string input)
{
    (void) g;
    std::istringstream is(input);
    std::string command;
    std::getline(is, command, ' ');
    cout << command;
}

int main()
{
    std::string input;
    std::getline(cin, input);
    handle_user_input(nullptr, input);
 
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

    Graph *g = new GRAPH_IMPL(vertex_count, edge_count, edges);
    auto comps = kosaraju(*g);
    cout << "The strongly connected components are: " << std::endl;
    for (auto comp : comps)
    {
        for (vertex v : comp)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    */
}