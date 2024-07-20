CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb
LDFLAGS = -pthread

BUILD_DIR = ../build
GRAPH_DIR = ../graph_library
LIB_DIR = ../pattern_library
BUILD_DIR = ../build

PATTERN_LIB = libpattern_library
GRAPH_LIB = libgraph
MAIN = main
EXE = kosaraju

all: kosaraju

$(BUILD_DIR):
	-mkdir -p $@

kosaraju: $(MAIN).o $(BUILD_DIR)/$(GRAPH_LIB).a $(BUILD_DIR)/$(PATTERN_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(GRAPH_LIB).a:
	make -C $(GRAPH_DIR)

$(BUILD_DIR)/$(PATTERN_LIB).a:
	make -C $(LIB_DIR)

clean:
	rm -f $(EXE) $(MAIN).o

.PHONY: clean all
.SUFFIXES:
