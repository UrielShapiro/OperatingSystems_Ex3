#ifndef KOSARAJU_HPP
#define KOSARAJU_HPP
#include <set>
#include "Graph.hpp"

std::set<std::set<vertex>> kosaraju(const Graph &g);

#endif