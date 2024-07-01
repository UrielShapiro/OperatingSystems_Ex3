CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17

MAIN = main
LIB = kosaraju

GRAPH = Graph
GRAPH_IMPL = AdjacencyGraph

HEADERS = $(GRAPH).hpp $(GRAPH_IMPL).hpp $(LIB).hpp

EXE = KosarajuServer

all: $(EXE)

$(EXE): $(MAIN).o $(LIB).o $(GRAPH_IMPL).o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB).o: $(LIB).cpp $(LIB).hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	

%Graph.o: %Graph.cpp %Graph.hpp $(GRAPH).hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean all profile
.SUFFIXES:

clean: 
	rm -rf *.o $(EXE)
