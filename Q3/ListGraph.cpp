#include "ListGraph.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

using std::vector, std::pair, std::list;

ListGraph::ListGraph(size_t vertices, vector<pair<vertex, vertex>> edges) : adjacency_list(vertices + 1, list<vertex>())
{
    for (auto p : edges)
    {
        using std::operator""s;
        if (p.first > adjacency_list.capacity() || p.second > adjacency_list.capacity() || p.first <= 0 || p.second <= 0)
            throw std::invalid_argument("Error: Invalid Argument, Maximum value is: " + std::to_string(adjacency_list.capacity()) + ", got "s + std::to_string(p.first) + ", " + std::to_string(p.second));
        this->adjacency_list.at(p.first).push_back(p.second);
    }
}

size_t ListGraph::get_vertex_count() const
{
    return this->adjacency_list.size() - 1;
}

bool ListGraph::edge_exists(const vertex src, const vertex dst) const
{
    return std::find(this->adjacency_list.at(src).begin(),
                     this->adjacency_list.at(src).end(), dst) != this->adjacency_list.at(src).end();
}

list<vertex> ListGraph::get_neighbors(const vertex v) const
{
    return this->adjacency_list.at(v - 1);
}

bool ListGraph::add_edge(const vertex src, const vertex dst)
{
    bool output = std::find(this->adjacency_list.at(src).begin(),
                            this->adjacency_list.at(src).end(), dst) == this->adjacency_list.at(src).end();
    if (output)
        this->adjacency_list.at(src).push_back(dst);

    return output;
}

bool ListGraph::remove_edge(const vertex src, const vertex dst)
{
    bool output = std::find(this->adjacency_list.at(src).begin(),
                            this->adjacency_list.at(src).end(), dst) != this->adjacency_list.at(src).end();
    if (output)
        this->adjacency_list.at(src).remove(dst);

    return output;
}

 list<vertex> ListGraph::get_incoming_neighbors(const vertex dst) const
 {
    list<vertex> output;
    for (size_t i = 1; i < adjacency_list.size(); ++i)
    {
        list<vertex> l = adjacency_list.at(i);
        if(std::find(l.begin(),l.end(), dst) != l.end())
            output.push_back(i);
    }
    return output;
 }
