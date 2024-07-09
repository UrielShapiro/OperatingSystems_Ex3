CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17
PROFILING = ProfilingTest
PROFILINGFLAGS = -pg
GRAPH_DIR = ../graph_library
GRAPH_LIB = libgraph
BUILD_DIR = ../build

all: $(PROFILING)

$(BUILD_DIR):
	-mkdir -p $@

gprof: $(PROFILING)
	./$(PROFILING)
	gprof ./$(PROFILING) > GraphProfiling.txt
	-rm -rf gmon.out

$(PROFILING): $(PROFILING).o $(BUILD_DIR)/$(GRAPH_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PROFILINGFLAGS) -o $@ $^

$(PROFILING).o: $(PROFILING).cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(GRAPH_LIB).a:
	make -C $(GRAPH_DIR)

clean:
	rm -rf *.o profile $(PROFILING) gmon.out GraphProfiling.txt

.PHONY: all gprof clean
.SUFFIXES:
