#include "AdjacencyGraph.hpp"
#include <stdexcept>

using std::vector, std::pair;

AdjacencyGraph::AdjacencyGraph(size_t vertices, vector<pair<vertex, vertex>> edges)
{
    this->adjacency_matrix.reserve(vertices);   // Reserve space for the vertices in the adjacency matrix
    vector<bool> empty_row(vertices, false);    // Initialize a row of the adjacency matrix with no neighbours

    for (size_t i = 0; i < vertices; i++)
    {
        this->adjacency_matrix.push_back(empty_row); // Initialize all vertices to have no neighbours
    }

    // Set neighbours on the adjacency matrix
    for (auto p : edges)
    {
        adjacency_matrix.at(p.first - 1).at(p.second - 1) = true;
    }
}

size_t AdjacencyGraph::get_vertex_count() const
{
    return adjacency_matrix.size();
}

bool AdjacencyGraph::edge_exists(const vertex src, const vertex dst) const
{
    if (src > this->get_vertex_count() || dst > this->get_vertex_count() || src <= 0 || dst <= 0)
        throw std::out_of_range("Error: Invalid argument passed, One of the vertices does not exist");
    return this->adjacency_matrix.at(src - 1).at(dst - 1);
}

std::list<vertex> AdjacencyGraph::get_neighbors(const vertex v) const
{
    if(v > this->get_vertex_count() || v <= 0)
        throw std::out_of_range("Error: Invalid argument passed, Vertex does not exist");
    std::list<vertex> output;
    for (size_t i = 0; i < this->get_vertex_count(); i++)
    {
        if (this->adjacency_matrix.at(v - 1).at(i)) // Is true == there's an edge between them
            output.push_back(i + 1);
    }
    return output;
}

bool AdjacencyGraph::add_edge(const vertex src, const vertex dst)
{
    if (src > this->get_vertex_count() || dst > this->get_vertex_count() || src <= 0 || dst <= 0)
        throw std::out_of_range("Error: Can't add this edge, One of the vertices does not exist");

    bool output = !this->adjacency_matrix.at(src - 1).at(dst - 1);
    this->adjacency_matrix.at(src - 1).at(dst - 1) = true;
    return output;
}

bool AdjacencyGraph::remove_edge(const vertex src, const vertex dst)
{
    if (src > this->get_vertex_count() || dst > this->get_vertex_count() || src <= 0 || dst <= 0)
        throw std::out_of_range("Error: Can't remove this edge, One of the vertices does not exist");

    bool output = this->adjacency_matrix.at(src - 1).at(dst - 1);
    this->adjacency_matrix.at(src - 1).at(dst - 1) = false;
    return output;
}

std::list<vertex> AdjacencyGraph::get_incoming_neighbors(const vertex dst) const
{
    if(dst > this->get_vertex_count() || dst <= 0)
        throw std::out_of_range("Error: Vertex does not exist");

    std::list<vertex> output;
    for (size_t i = 0; i < this->get_vertex_count(); i++)
    {
        if (this->adjacency_matrix.at(i).at(dst - 1))
            output.push_back(i + 1);
    }

    return output;
}
