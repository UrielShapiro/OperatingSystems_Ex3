CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb
AR = ar
ARFLAGS = rcs

BUILD_DIR = ../build
LIB = libgraph
ADJACENCY = AdjacencyGraph
LIST = ListGraph
GRAPH = Graph
KOSARAJU = kosaraju


all: $(BUILD_DIR)/$(LIB).a

$(BUILD_DIR):
	-mkdir -p $@

$(BUILD_DIR)/$(KOSARAJU).o: $(KOSARAJU).cpp $(KOSARAJU).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(ADJACENCY).o: $(ADJACENCY).cpp $(GRAPH).hpp $(ADJACENCY).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(LIST).o: $(LIST).cpp $(GRAPH).hpp $(LIST).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(LIB).a: $(BUILD_DIR)/$(ADJACENCY).o $(BUILD_DIR)/$(LIST).o $(BUILD_DIR)/$(KOSARAJU).o | $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(BUILD_DIR)/$(LIST).o $(BUILD_DIR)/$(ADJACENCY).o $(BUILD_DIR)/$(KOSARAJU).o $(BUILD_DIR)/$(LIB).a

.PHONY: all clean
.SUFFIXES:
