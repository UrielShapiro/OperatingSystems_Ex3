#ifndef KOSARAJU_HPP
#define KOSARAJU_HPP
#include <set>
#include "../graph_library/Graph.hpp"

std::set<std::set<vertex>> kosaraju_deque(const Graph &g);

#endif