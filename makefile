.PHONY: test

TEST_DIR := Tests
LEXER := cq_lexer
PARSER := cq_parser

cq_parser: $(LEXER).l $(PARSER).y
	bison -d $(PARSER).y
	flex -o $(LEXER).yy.c $(LEXER).l
	clang -o $@ $(PARSER).tab.c symtab.c $(LEXER).yy.c -ll
	@rm $(LEXER).yy.c $(PARSER).tab.c $(PARSER).tab.h

test:
	@echo "Running tests..."; \

	@for file in $(TEST_DIR)/test_decl_*.cq; do \
		./$(PARSER) $$file; \
		if [ $$? -eq 1 ]; then \
			echo "$(PARSER) returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Declarations passed."; \

	@for file in $(TEST_DIR)/test_def_*.cq; do \
		./$(PARSER) $$file; \
		if [ $$? -eq 1 ]; then \
			echo "$(PARSER) returned 1 for file $$(basename $$file)"; \
			exit 1; \
		fi; \
	done; \
	echo "- Definitions passed."

clean:
	@rm -f $(PARSER) $(PARSER).output symtab_dump.out $(PARSER).tab.c $(PARSER).tab.h $(LEXER).yy.c
