.PHONY: test

TEST_DIR := Tests
LEXER := cq_lexer
PARSER := cq_parser

all: $(LEXER).l $(PARSER).y symtab.c ast.c
	bison -d $(PARSER).y
	flex -o $(LEXER).yy.c $(LEXER).l
	clang -o $(PARSER) $(PARSER).tab.c symtab.c ast.c $(LEXER).yy.c -ll
	@rm $(LEXER).yy.c $(PARSER).tab.c $(PARSER).tab.h

example:
	@./$(PARSER) example.cq --dump
	@cat symtab_dump.out

test:
	@echo "Running tests..."; \

	@for file in $(TEST_DIR)/test_decl_*.cq; do \
		./$(PARSER) $$file; \
		if [ $$? -ne 0 ]; then \
			echo "$(PARSER) returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Declarations passed."; \

	@for file in $(TEST_DIR)/test_def_*.cq; do \
		./$(PARSER) $$file; \
		if [ $$? -ne 0 ]; then \
			echo "$(PARSER) returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Definitions passed."

clean:
	@rm -f $(PARSER) $(PARSER).output symtab_dump.out $(PARSER).tab.c $(PARSER).tab.h $(LEXER).yy.c
