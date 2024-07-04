CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb

SRCS_DIR = sources
BUILD_DIR = ./build
LIB_DIR = ./pattern_library
INCLUDE_DIR = ./include

all: Q6/Kosaraju

$(BUILD_DIR):
	-mkdir $@

$(BUILD_DIR)/reactor.o: $(LIB_DIR)/reactor.cpp $(LIB_DIR)/reactor.hpp $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(BUILD_DIR)/proactor.o: $(LIB_DIR)/proactor.cpp $(LIB_DIR)/proactor.hpp $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/kosaraju.o: $(SRCS_DIR)/kosaraju.cpp $(INCLUDE_DIR)/kosaraju.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/AdjacencyGraph.o: $(SRCS_DIR)/AdjacencyGraph.cpp $(INCLUDE_DIR)/Graph.hpp $(INCLUDE_DIR)/AdjacencyGraph.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/pattern_library.a: $(BUILD_DIR)/reactor.o # $(BUILD_DIR)/proactor.o
	ar -crs $@ $^

Q6/Kosaraju: Q6/main.o $(BUILD_DIR)/pattern_library.a $(BUILD_DIR)/kosaraju.o $(BUILD_DIR)/AdjacencyGraph.o
	$(CXX) $(CXXFLAGS) -o $@ $^

Q6/main.o: Q6/main.cpp $(INCLUDE_DIR)/AdjacencyGraph.hpp $(INCLUDE_DIR)/Graph.hpp $(INCLUDE_DIR)/kosaraju.hpp $(LIB_DIR)/reactor.cpp

clean:
	rm -rf $(BUILD_DIR) Q6/Kosaraju	Q6/*.o

.PHONY: clean all