#include "AdjacencyGraph.hpp"

AdjacencyGraph::AdjacencyGraph(size_t vertices, size_t num_of_edges, vector<pair<vertex, vertex>> edges)
{
    adjacency_matrix.reserve(vertices + 1);
    vector<bool> empty_list(vertices + 1, false);

    for (size_t i = 0; i < vertices + 1; i++)
    {
        adjacency_matrix.push_back(empty_list);
    }

    for (size_t i = 0; i < num_of_edges; i++)
    {
        for (pair p : edges)
        {
            adjacency_matrix.at(p.first + 1).at(p.second + 1) = true;
        }
    }
}

size_t AdjacencyGraph::get_vertex_count() const
{
    return adjacency_matrix.size() - 1;
}

bool AdjacencyGraph::edge_exists(const vertex src, const vertex dst) const
{
    return this->adjacency_matrix.at(src + 1).at(dst + 1);
}

std::list<vertex> AdjacencyGraph::get_neighbors(const vertex v) const
{
    std::list<vertex> output;
    for (size_t i = 1; i < this->get_vertex_count(); i++)
    {
        if (this->adjacency_matrix.at(v).at(i)) // Is true == there's an edge between them
        {
            output.push_back(i + 1);
        }
    }
    return output;
}

bool AdjacencyGraph::add_edge(const vertex src, const vertex dst)
{
    if (src > this->get_vertex_count() || dst > this->get_vertex_count())
        return false;

    bool output = this->adjacency_matrix.at(src + 1).at(dst + 1) == false ? true : false;
    this->adjacency_matrix.at(src + 1).at(dst + 1) = true;
    return output;
}

bool AdjacencyGraph::remove_edge(const vertex src, const vertex dst)
{
    if (src > this->get_vertex_count() || dst > this->get_vertex_count())
        return false;

    bool output = this->adjacency_matrix.at(src + 1).at(dst + 1) == true ? true : false;
    this->adjacency_matrix.at(src + 1).at(dst + 1) = false;
    return output;
}
