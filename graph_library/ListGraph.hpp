#pragma once

#include "Graph.hpp"
#include <list>
#include <vector>

class ListGraph : public Graph
{
private:
    /**
     * @brief The adjacency list of the graph. adjacency_list[i] is a list of vertices that are neighbors of vertex i
     */
    std::vector<std::list<vertex>> adjacency_list;

public:
    /**
     * @brief Construct a new List Graph object
     * @param vertices The number of vertices in the graph
     * @param edges The edges of the graph (pair of a source vertex and a destination vertex)
     */
    ListGraph(size_t vertices, std::vector<std::pair<vertex, vertex>> edges);
    virtual size_t get_vertex_count() const override;
    virtual bool edge_exists(const vertex src, const vertex dst) const override;
    virtual std::list<vertex> get_neighbors(const vertex v) const override;
    virtual bool add_edge(const vertex src, const vertex dst) override;
    virtual bool remove_edge(const vertex src, const vertex dst) override;
    virtual std::list<vertex> get_incoming_neighbors(const vertex dst) const override;
};
