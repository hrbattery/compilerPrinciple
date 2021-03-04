dianchi: wordCounter
	./wordCounter < wordCounter.l

wordCounter: lex.yy.c
	gcc -o $@ $<

lex.yy.c: wordCounter.l
	flex $<