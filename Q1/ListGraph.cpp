#include "ListGraph.hpp"
#include <algorithm>
#include <stdexcept>

ListGraph::ListGraph(size_t vertices, vector<pair<vertex, vertex>> edges)
{
    this->adjacency_list.reserve(vertices + 1);

    for (pair p : edges)
    {
        if (p.first > adjacency_list.size() || p.second > adjacency_list.size() || p.first <= 0 || p.second <= 0)
            throw std::invalid_argument("Error: Invalid Argument");
        this->adjacency_list.at(p.first).push_back(p.second);
    }
}

size_t ListGraph::get_vertex_count() const
{
    return this->adjacency_list.size();
}

bool ListGraph::edge_exists(const vertex src, const vertex dst) const
{
    return std::find(this->adjacency_list.at(src + 1).begin(),
                     this->adjacency_list.at(src + 1).end(), dst) != this->adjacency_list.at(src).end();
}

std::list<vertex> ListGraph::get_neighbors(const vertex v) const
{
    return this->adjacency_list.at(v + 1);
}

bool ListGraph::add_edge(const vertex src, const vertex dst)
{
    bool output = std::find(this->adjacency_list.at(src + 1).begin(),
                            this->adjacency_list.at(src + 1).end(), dst) == this->adjacency_list.at(src + 1).end();
    if (output)
        this->adjacency_list.at(src + 1).push_back(dst);

    return output;
}

bool ListGraph::remove_edge(const vertex src, const vertex dst)
{
    bool output = std::find(this->adjacency_list.at(src + 1).begin(),
                            this->adjacency_list.at(src + 1).end(), dst) != this->adjacency_list.at(src + 1).end();
    if (output)
        this->adjacency_list.at(src + 1).remove(dst);

    return output;
}
