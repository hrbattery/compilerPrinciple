CC = gcc
OUT = tcc
OBJ = lex.yy.c y.tab.o
SCANNER = scanner.l
PARSER = parser.y

build: $(OUT)

run: $(OUT)
	./$(OUT) < test.c > test.asm

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h y.output $(OUT)

$(OUT):	$(OBJ)
	$(CC) -o $(OUT) $(OBJ)

lex.yy.c: $(SCANNER)
	flex $<

y.tab.c: $(PARSER)
	bison -vdty $<