CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -ggdb
LDFLAGS = -pthread
AR = ar

TEST1 = reactor_test
TEST2 = proactor_test
REACTOR = reactor
PROACTOR = proactor
LIB = libpattern_library
BUILD_DIR = ../build

all: $(TEST1) $(TEST2)

$(BUILD_DIR):
	-mkdir -p $@

$(TEST1): $(TEST1).cpp $(BUILD_DIR)/$(LIB).a	| $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/$(LIB).a $(LDFLAGS)

$(TEST2): $(TEST2).cpp $(BUILD_DIR)/$(LIB).a	| $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/$(LIB).a $(LDFLAGS)

$(BUILD_DIR)/$(REACTOR).o: $(REACTOR).cpp $(REACTOR).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(PROACTOR).o: $(PROACTOR).cpp $(PROACTOR).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(LIB).a: $(BUILD_DIR)/$(REACTOR).o $(BUILD_DIR)/$(PROACTOR).o | $(BUILD_DIR)
	$(AR) rcs $@ $^


clean:
	rm -f $(TEST1) $(TEST2) $(BUILD_DIR)/$(REACTOR).o $(BUILD_DIR)/$(PROACTOR).o $(BUILD_DIR)/$(LIB).a

.PHONY: all clean
.SUFFIXES:
