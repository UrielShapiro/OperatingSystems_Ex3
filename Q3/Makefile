CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17
GRAPH_DIR = ../graph_library
GRAPH_LIB = libgraph
BUILD_DIR = ../build

# GRAPH_IMPL = ListGraph
GRAPH_IMPL = AdjacencyGraph
MAIN = main
EXE = Kosaraju

all: $(EXE)

$(BUILD_DIR):
	-mkdir -p $@

$(EXE): $(MAIN).o $(BUILD_DIR)/$(GRAPH_LIB).a
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp $(BUILD_DIR)/$(GRAPH_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(GRAPH_LIB).a:
	make -C $(GRAPH_DIR)

.PHONY: clean all
.SUFFIXES:

clean: 
	rm -rf *.o $(EXE) $(Profiling) gmon.out
	