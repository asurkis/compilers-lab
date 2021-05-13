build: ast.c ast.h parser tokenizer
	cc ast.c lex.yy.c -lfl -o main

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h main

parser: parser.y
	bison -d parser.y

tokenizer: lexer.l
	flex lexer.l