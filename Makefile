CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb

QS = Q2 Q3 Q6 Q7
QS_all = $(foreach q, $(QS), $(q)_all)
QS_clean = $(foreach q, $(QS), $(q)_clean)

BUILD_DIR = ./build
LIB_DIR = ./pattern_library
GRAPH_DIR = ./graph_library

all: $(QS_all)

$(QS_all):
	$(MAKE) -C $(@:_all=) all

$(QS_clean):
	$(MAKE) -C $(@:_clean=) clean

clean: $(QS_clean)

.PHONY: clean all $(QS_all) $(QS_clean)