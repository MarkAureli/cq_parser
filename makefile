.PHONY:test

TEST_DIR := Tests
EXECUTABLE := ./cq_parser

cq_parser: cq_lexer.l cq_parser.y
	bison -d cq_parser.y
	flex -o cq_lex.yy.c cq_lexer.l
	clang -o $@ cq_parser.tab.c cq_lex.yy.c -ll

test:
	@echo "Running tests..."; \

	@for file in $(TEST_DIR)/test_decl_*.cq; do \
		$(EXECUTABLE) $$file; \
		if [ $$? -eq 1 ]; then \
			echo "cq_parser returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Declarations passed."; \

	@for file in $(TEST_DIR)/test_def_*.cq; do \
		$(EXECUTABLE) $$file; \
		if [ $$? -eq 1 ]; then \
			echo "cq_parser returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Definitions passed."
