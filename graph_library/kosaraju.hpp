#ifndef KOSARAJU_HPP
#define KOSARAJU_HPP
#include <set>
#include "Graph.hpp"

/**
 * @brief Find the strongly connected components of a graph using Kosaraju's algorithm
 * @param g The graph to find the strongly connected components of
 * @return A set of sets of vertices, each set represents a strongly connected component
 */
std::set<std::set<vertex>> kosaraju(const Graph &g);

#endif
