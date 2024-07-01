#include "Graph.hpp"
#include <vector>

#define MAX_SEGMENT_SIZE 65535

using std::pair, std::vector;

struct GraphInput
{
    size_t vertex_count;
    size_t edges_count;
    std::vector<std::pair<vertex, vertex>> edges;
};

GraphInput initialize_graph(int userfd);
char* handle_client_input(int fd);