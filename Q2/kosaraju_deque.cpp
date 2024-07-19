#include "kosaraju_deque.hpp"
#include <deque>
#include <set>
#include <map>
#include <algorithm>

void deque_visit(const Graph &g, std::map<vertex, bool> &visited, std::deque<vertex> &l, vertex u)
{
    if (visited[u])
        return;
    visited[u] = true;
    for (auto v : g.get_neighbors(u))
        deque_visit(g, visited, l, v);
    l.push_front(u);
}

void deque_assign(const Graph &g, std::map<vertex, std::set<vertex>> &assignments, std::map<vertex, bool> &assigned, vertex u, vertex root)
{
    if (assigned[u])
        return;
    assignments[root].insert(u);
    assigned[u] = true;
    for (vertex v : g.get_incoming_neighbors(u))
        deque_assign(g, assignments, assigned, v, root);
}

std::set<std::set<vertex>> kosaraju_deque(const Graph &g)
{
    size_t n = g.get_vertex_count();
    std::map<vertex, bool> visited;
    std::deque<vertex> l;
    for (vertex i = 1; i <= n; ++i)
        visited[i] = false;
    for (vertex u = 1; u <= n; ++u)
        deque_visit(g, visited, l, u);
    std::map<vertex, std::set<vertex>> assignments;
    std::map<vertex, bool> assigned;
    for (vertex i = 1; i <= n; ++i)
        assigned[i] = false;
    for (auto u : l)
    {
        deque_assign(g, assignments, assigned, u, u);
    }
    std::set<std::set<vertex>> result;
    for (auto &[_, comp] : assignments)
    {
        result.insert(comp);
    }
    return result;
}
