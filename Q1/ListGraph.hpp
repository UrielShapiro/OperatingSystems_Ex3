#include "Graph.hpp"
#include <list>
#include <vector>

using std::list, std::vector, std::pair;
class ListGraph : public Graph
{
private:
    vector<list<vertex>> adjacency_list;

public:
    ListGraph(size_t vertices, vector<pair<vertex, vertex>> edges);
    virtual size_t get_vertex_count() const override;
    virtual bool edge_exists(const vertex src, const vertex dst) const override;
    virtual std::list<vertex> get_neighbors(const vertex v) const override;
    virtual bool add_edge(const vertex src, const vertex dst) override;
    virtual bool remove_edge(const vertex src, const vertex dst) override;
};