CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g

main = main

SRCS = $(wildcard *.cpp)
SRC := $(filter-out $(main).cpp, $(SRCS))

OBJECTS = $(SRCS:.cpp=.o)

EXE = Kosaraju

all: $(EXE)

$(EXE): $(main).cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
