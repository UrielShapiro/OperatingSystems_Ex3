CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17
PROFILINGFLAGS = -pg

MAIN = main
LIB = kosaraju

GRAPH = Graph
GRAPH_IMPL = AdjacencyGraph
Profiling = ProfilingTest

GRAPHS = AdjacencyGraph ListGraph
# GRAPH_IMPL = ListGraph
HEADERS = $(GRAPH).hpp $(GRAPH_IMPL).hpp $(LIB).hpp

EXE = Kosaraju

all: $(EXE)

$(EXE): $(MAIN).o $(LIB).o $(GRAPH_IMPL).o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB).o: $(LIB).cpp $(LIB).hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%Graph.o: %Graph.cpp %Graph.hpp $(GRAPH).hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

profile: $(Profiling)
	./$(Profiling)
	gprof ./$(Profiling) > GraphProfiling.txt
	-rm -rf gmon.out

$(Profiling): $(Profiling).cpp $(foreach g,$(GRAPHS),$g.o) $(LIB).o
	$(CXX) $(CXXFLAGS) $(PROFILINGFLAGS) -o $@ $^

.PHONY: clean all profile
.SUFFIXES:

clean: 
	rm -rf *.o $(EXE) $(Profiling) gmon.out
