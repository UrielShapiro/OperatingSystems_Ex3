#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <list>
#include <map>
#include "Graph.hpp"
#include "AdjacencyGraph.hpp"

using std::cout;
using std::cin;

std::set<std::set<vertex>> kosaraju(const Graph &g)
{
    size_t n = g.get_vertex_count();
    std::map<vertex, bool> visited;
    for (vertex i = 1; i <= n; ++i)
    {
        visited.
    }
    std::list<vertex> l;
    auto visit = [g, visited, l, &visit](vertex u)
    {
        if (visited[u]) return;
        visited[u] = true;
        for (auto v : g.get_neighbors(u))
            visit(v);
        l.push_front(u);
    };
    for (vertex u = 1; u <= n; ++u)
        visit(u);
}

int main()
{
    size_t vertex_count, edge_count;
    std::vector<std::pair<vertex, vertex>> edges;
    cout << "Enter number of vertex_count:";
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

    Graph *g = new AdjacencyGraph(vertex_count, edge_count, edges);
}