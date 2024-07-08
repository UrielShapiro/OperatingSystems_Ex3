CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17
PROFILING = ProfilingTest
PROFILINGFLAGS = -pg
LIB_DIR = ../graph_library
BUILD_DIR = ../build

all: profile

$(BUILD_DIR):
	-mkdir $@

$(BUILD_DIR)/AdjacencyGraph.o: $(LIB_DIR)/AdjacencyGraph.cpp $(LIB_DIR)/AdjacencyGraph.hpp $(LIB_DIR)/Graph.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/ListGraph.o: $(LIB_DIR)/ListGraph.cpp $(LIB_DIR)/ListGraph.hpp $(LIB_DIR)/Graph.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(BUILD_DIR)/kosaraju.o: $(LIB_DIR)/kosaraju.cpp $(LIB_DIR)/kosaraju.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

profile: $(PROFILING)		
	./$(PROFILING)
	gprof ./$(PROFILING) > GraphProfiling.txt
	-rm -rf gmon.out

$(PROFILING): $(PROFILING).cpp $(BUILD_DIR)/ListGraph.o $(BUILD_DIR)/AdjacencyGraph.o $(BUILD_DIR)/kosaraju.o	
	$(CXX) $(CXXFLAGS) $(PROFILINGFLAGS) -o $@ $^

clean:
	rm -rf *.o profile $(Profiling) gmon.out
