out:		scanner

scanner:	scanner.cpp
			g++ -o $@ $< -std=c++11

clean:
			rm scanner result.txt

test:
			./scanner test.txt > result.txt