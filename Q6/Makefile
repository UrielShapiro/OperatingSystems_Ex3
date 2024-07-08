CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb

BUILD_DIR = ../build
GRAPH_DIR = ../graph_library
LIB_DIR = ../pattern_library

PATTERN_LIB = libpattern_library
GRAPH_LIB = libgraph
MAIN = main
EXE = kosaraju

all: $(EXE)

$(BUILD_DIR):
	-mkdir -p $@

$(EXE): $(MAIN).o $(BUILD_DIR)/$(GRAPH_LIB).a $(BUILD_DIR)/$(PATTERN_LIB).a | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(GRAPH_LIB).a:
	make -C $(GRAPH_DIR)

$(BUILD_DIR)/$(PATTERN_LIB).a:
	make -C $(LIB_DIR)

clean:
	rm -f $(MAIN).o $(EXE)

.PHONY: clean all
