#pragma once

#include <list>

using vertex = size_t;

class Graph
{
public:
    virtual size_t get_vertex_count() const = 0;
    virtual bool edge_exists(const vertex src, const vertex dst) const = 0;
    virtual std::list<vertex> get_neighbors(const vertex v) const = 0;
    virtual bool add_edge(const vertex src, const vertex dst) = 0;
    virtual bool remove_edge(const vertex src, const vertex dst) = 0;
};