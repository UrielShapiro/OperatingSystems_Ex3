#include "kosaraju.hpp"
#include <set>
#include <map>
#include <algorithm>

/**
 * @brief Perform a DFS on the graph and store the vertices in topological order in the list l
 * @param g The graph to perform the DFS on
 * @param visited A map that stores whether a vertex has been visited or not
 * @param l The list to store the vertices in topological order
 * @param u The current vertex
 */
void visit(const Graph &g, std::map<vertex, bool> &visited, std::list<vertex> &l, vertex u)
{
    if (visited[u])
        return;
    visited[u] = true;
    for (auto v : g.get_neighbors(u))
        visit(g, visited, l, v);    // Visit all neighbors of u
    l.push_front(u);    // Add u to the front of the list
}

/**
 * @brief Assign vertices to strongly connected components
 * @param g The graph
 * @param assignments A map that stores the vertices in the same strongly connected component as the root vertex
 * @param assigned A map that stores whether a vertex has been assigned to a strongly connected component or not
 * @param u The current vertex
 */
void assign(const Graph &g, std::map<vertex, std::set<vertex>> &assignments, std::map<vertex, bool> &assigned, vertex u, vertex root)
{
    if (assigned[u])
        return;
    assignments[root].insert(u);    // Add u to the set of vertices in the same strongly connected component as root
    assigned[u] = true;

    // Recursively assign all incoming neighbors of u to the same strongly connected component
    for (vertex v : g.get_incoming_neighbors(u))
        assign(g, assignments, assigned, v, root);
}

std::set<std::set<vertex>> kosaraju(const Graph &g)
{
    size_t n = g.get_vertex_count();
    std::map<vertex, bool> visited; // visited[i] = true if vertex i has been visited
    std::list<vertex> l; // l is the list of vertices in topological order
    
    // Initialize visited vector to false
    for (vertex i = 1; i <= n; ++i)
        visited[i] = false;

    // Perform a DFS on the graph and store the vertices in topological order
    for (vertex u = 1; u <= n; ++u)
        visit(g, visited, l, u);
    std::map<vertex, std::set<vertex>> assignments; // assignments[root] = set of vertices in the same strongly connected component as root
    std::map<vertex, bool> assigned;    // assigned[i] = true if vertex i has been assigned to a strongly connected component
    
    // Initialize assigned vector to false
    for (vertex i = 1; i <= n; ++i)
        assigned[i] = false;
    
    // Assign vertices to strongly connected components
    for (auto u : l)
    {
        assign(g, assignments, assigned, u, u);
    }
    std::set<std::set<vertex>> result;  // The set of sets of vertices, each set represents a strongly connected component

    // Add all strongly connected components to the result set
    for (auto &[_, comp] : assignments)
    {
        result.insert(comp);
    }

    return result;
}
