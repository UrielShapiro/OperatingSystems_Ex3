#pragma once

#include <list>

using vertex = size_t;  // Type alias for the vertex types

/**
 * @brief A graph interface that defines the basic operations that a graph should support
 */
class Graph
{
public:
    virtual ~Graph() = default;

    /**
     * @brief Get the number of vertices in the graph
     */
    virtual size_t get_vertex_count() const = 0;

    /**
     * @brief Check if an edge exists between two vertices
     * @param src The source vertex
     * @param dst The destination vertex
     * @return true if an edge exists between the two vertices, false otherwise
     */
    virtual bool edge_exists(const vertex src, const vertex dst) const = 0;

    /**
     * @brief Get the neighbors of a vertex
     * @param v The vertex to get the neighbors of
     * @return A list of vertices that are neighbors of the given vertex
     */
    virtual std::list<vertex> get_neighbors(const vertex v) const = 0;

    /**
     * @brief Add an edge between two vertices
     * @param src The source vertex
     * @param dst The destination vertex
     * @return true if the edge was added successfully, false otherwise
     */
    virtual bool add_edge(const vertex src, const vertex dst) = 0;

    /**
     * @brief Remove an edge between two vertices
     * @param src The source vertex
     * @param dst The destination vertex
     * @return true if the edge was removed successfully, false otherwise
     */
    virtual bool remove_edge(const vertex src, const vertex dst) = 0;

    /**
     * @brief Get the incoming neighbors of a vertex (who has an edge to the given vertex)
     * @param dst The vertex to get the incoming neighbors of
     * @return A list of vertices that are incoming neighbors of the given vertex
     */
    virtual std::list<vertex> get_incoming_neighbors(const vertex dst) const = 0;
};
