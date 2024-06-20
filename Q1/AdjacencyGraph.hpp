#include <list>
#include <vector>
#include "Graph.hpp"

using std::vector, std::pair;

class AdjacencyGraph : public Graph
{
private:
    vector<vector<bool>> adjacency_matrix;

public:
    AdjacencyGraph(size_t vertices, size_t num_of_edges, vector<pair<vertex, vertex>> edges);
    virtual size_t get_vertex_count() const override;
    virtual bool edge_exists(const vertex src, const vertex dst) const override;
    virtual std::list<vertex> get_neighbors(const vertex v) const override;
    virtual bool add_edge(const vertex src, const vertex dst) override;
    virtual bool remove_edge(const vertex src, const vertex dst) override;
};