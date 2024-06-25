CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17

main = main

GRAPH = Graph
GRAPH_IMPL = AdjacencyGraph
# GRAPH_IMPL = ListGraph
HEADERS = $(GRAPH).hpp $(GRAPH_IMPL).hpp

EXE = Kosaraju

all: $(EXE)

$(EXE): $(main).o $(GRAPH_IMPL).o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(main).o: $(main).cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(GRAPH_IMPL).o: $(GRAPH_IMPL).cpp $(GRAPH_IMPL).hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean all
.SUFFIXES:

clean: 
	rm -rf *.o $(EXE)
