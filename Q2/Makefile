CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17
PROFILING = GraphTypeProfiling
KOSARAJU_PROFILING = KosarajuProfiling
PROFILINGFLAGS = -pg
GRAPH_DIR = ../graph_library
GRAPH_LIB = libgraph
BUILD_DIR = ../build
DEQUE = kosaraju_deque

all: $(PROFILING) $(KOSARAJU_PROFILING)

$(BUILD_DIR):
	-mkdir -p $@

gprof: $(PROFILING) $(KOSARAJU_PROFILING)
	./$(PROFILING)
	gprof ./$(PROFILING) > GraphProfiling.txt
	-rm -rf gmon.out
	./$(KOSARAJU_PROFILING)
	gprof ./$(KOSARAJU_PROFILING) > KosarajuProfiling.txt
	-rm -rf gmon.out

$(PROFILING): $(PROFILING).o $(BUILD_DIR)/$(GRAPH_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PROFILINGFLAGS) -o $@ $^

$(PROFILING).o: $(PROFILING).cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(KOSARAJU_PROFILING): $(KOSARAJU_PROFILING).o $(DEQUE).o $(BUILD_DIR)/$(GRAPH_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PROFILINGFLAGS) -o $@ $^

$(KOSARAJU_PROFILING).o: $(KOSARAJU_PROFILING).cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEQUE).o: $(DEQUE).cpp $(DEQUE).hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(GRAPH_LIB).a:
	make -C $(GRAPH_DIR)

clean:
	rm -rf *.o profile $(PROFILING) $(KOSARAJU_PROFILING) gmon.out GraphProfiling.txt KosarajuProfiling.txt

.PHONY: all gprof clean
.SUFFIXES:
