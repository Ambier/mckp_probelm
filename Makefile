CC = g++
CFlags = -Wall -g

MCKP_MAIN_TARGET = mckp
MCKP_MAIN_DEPS = mckp.hh mckp_helpers.hh mckp.cpp

TEST_TARGET = mckp_test
TEST_DEPS = mckp.hh mckp_helpers.hh mckp_test.cpp

$(MCKP_MAIN_TARGET): $(MCKP_MAIN_DEPS)
	$(CC) $(CFlags) -o $(MCKP_MAIN_TARGET) $(MCKP_MAIN_DEPS)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_DEPS)
	$(CC) $(CFlags) -o $(TEST_TARGET) $(TEST_DEPS) -lgtest_main -lgtest -lpthread

clean:
	rm -rf *.o *.gch $(MCKP_MAIN_TARGET) $(TEST_TARGET)