CXX=g++
CFLAGS=--std=c++11

all: LALR

LALR: LALR.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm ./LALR

test: LALR 
	./LALR test.c > result.txt
