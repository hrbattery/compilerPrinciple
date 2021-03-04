out: 		scanner

scanner:	lex.yy.c token.h
		gcc -o $@ $<

lex.yy.c:	micro.l
		flex $<

test:		test.m
		./scanner < $<

clean:		
		rm scanner lex.yy.c