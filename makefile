cq_parser: cq_lexer.l cq_parser.y
	bison -d cq_parser.y --debug --verbose
	flex -o cq_lex.yy.c cq_lexer.l
	clang -o $@ cq_parser.tab.c cq_lex.yy.c -ll


