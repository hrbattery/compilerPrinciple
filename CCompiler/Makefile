CXX=g++
CFLAGS=--std=c++11

all: compi1er

compi1er: main.cpp
	$(CXX) $(CFLAGS) $< -o $@

# lalr.o: LALR.cpp
# 	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm ./LALR

test:  
	./LALR test.c > result.txt
