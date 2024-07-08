CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb
LDFLAGS = -pthread
AR = ar

TEST = test
REACTOR = reactor
PROACTOR = proactor
LIB = libpattern_library
BUILD_DIR = ../build

all: $(TEST) $(BUILD_DIR)/$(LIB).a

$(BUILD_DIR):
	-mkdir -p $@

$(TEST): $(TEST).cpp $(BUILD_DIR)/$(LIB).a	| $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/$(LIB).a $(LDFLAGS)

$(BUILD_DIR)/$(REACTOR).o: $(REACTOR).cpp $(REACTOR).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(PROACTOR).o: $(PROACTOR).cpp $(PROACTOR).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(LIB).a: $(BUILD_DIR)/$(REACTOR).o | $(BUILD_DIR)
	$(AR) rcs $@ $^

# $(PROACTOR).o

clean:
	rm -f $(TEST) $(BUILD_DIR)/$(REACTOR).o $(BUILD_DIR)/$(PROACTOR).o $(BUILD_DIR)/$(LIB).a

.PHONY: all clean
