CXX = g++
BISON = bison
FLEX = flex

CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Isrc
TARGET = config

SRC = src/parser.tab.cc src/lexer.yy.cc src/driver.cpp src/toml_writer.cpp src/main.cpp
OBJ = $(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRC)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

src/parser.tab.cc src/parser.tab.h: src/parser.y
	$(BISON) -d -o src/parser.tab.cc src/parser.y

src/lexer.yy.cc: src/lexer.l src/parser.tab.h
	$(FLEX) -o src/lexer.yy.cc src/lexer.l

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./tests/run_tests.sh

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f src/parser.tab.cc src/parser.tab.h src/lexer.yy.cc

.PHONY: all test clean
