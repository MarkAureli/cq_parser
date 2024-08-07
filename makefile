example_parser: example_lexer.l
	flex -o example_lex.yy.c example_lexer.l
	clang -o $@ example_lex.yy.c

q_parser: q_lexer.l q_parser.y
	bison -d q_parser.y
	flex -o q_lex.yy.c q_lexer.l
	clang -o $@ q_parser.tab.c q_lex.yy.c -ll

