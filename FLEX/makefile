run: example
	./example

example: lex.yy.c
	gcc -o $@ $<

lex.yy.c: example.l
	flex $<