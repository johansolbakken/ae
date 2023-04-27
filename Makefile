LEX=flex
YACC=bison
YFLAGS=
CPPFLAGS=-std=c++20 -g -Iinclude -Isrc

objects=src/parser.o src/scanner.o src/main.o src/tree.o src/generator.o src/preprocess.o src/utils.o src/symbol.o

# Aec
all: aec
aec: $(objects) 
	$(CXX) $(CPPFLAGS) $(objects) -o $@
src/scanner.cpp: src/scanner.lpp src/parser.cpp
	flex -o src/scanner.cpp src/scanner.lpp
src/parser.cpp: src/parser.ypp
	bison --defines=include/parser.hpp -o src/parser.cpp src/parser.ypp
clean:
	rm -rf $(objects)
purge: clean
	rm aec src/scanner.cpp include/parser.hpp src/parser.cpp