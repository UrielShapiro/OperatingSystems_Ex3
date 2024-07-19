#ifndef KOSARAJU_DEQUE_HPP
#define KOSARAJU_DEQUE_HPP
#include <set>
#include "../graph_library/Graph.hpp"

/**
 * @brief Find the strongly connected components of a graph using Kosaraju's algorithm with a deque
 */
std::set<std::set<vertex>> kosaraju_deque(const Graph &g);

#endif