out: 		examp

examp:	micro.tab.o lex.yy.o
		g++ -o $@ micro.tab.o lex.yy.o

micro.tab.o: micro.tab.cpp
		g++ -c $<

micro.tab.cpp: micro.ypp
		bison -vdt $<

lex.yy.o:	lex.yy.cpp 
		g++ -c $<

lex.yy.cpp: micro.lpp
		flex -o $@ $<

test:		test.m
		./examp < $<

clean:		
		rm examp lex.yy.o lex.yy.cpp micro.tab.o micro.tab.cpp micro.tab.hpp micro.output