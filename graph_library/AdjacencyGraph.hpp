#pragma once

#include <list>
#include <vector>
#include "Graph.hpp"

class AdjacencyGraph : public Graph
{
private:
    /**
     * @brief The adjacency matrix of the graph. adjacency_matrix[i][j] is true if there's an edge between vertex i + 1 and vertex j + 1
     */
    std::vector<std::vector<bool>> adjacency_matrix;

public:
    /**
     * @brief Construct a new Adjacency Graph object
     * @param vertices The number of vertices in the graph
     * @param edges The edges of the graph (pair of a source vertex and a destination vertex)
     */
    AdjacencyGraph(size_t vertices, std::vector<std::pair<vertex, vertex>> edges);
    virtual size_t get_vertex_count() const override;
    virtual bool edge_exists(const vertex src, const vertex dst) const override;
    virtual std::list<vertex> get_neighbors(const vertex v) const override;
    virtual bool add_edge(const vertex src, const vertex dst) override;
    virtual bool remove_edge(const vertex src, const vertex dst) override;
    virtual std::list<vertex> get_incoming_neighbors(const vertex dst) const override;
};
