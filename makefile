.PHONY: test

TEST_DIR := Tests
LEXER := cq_lexer
PARSER := cq_parser

all: $(LEXER).l $(PARSER).y symbol_table.c ast.c pars_utils.c
	bison -d $(PARSER).y
	flex -o $(LEXER).yy.c $(LEXER).l
	clang -o $(PARSER) $(PARSER).tab.c symbol_table.c ast.c pars_utils.c $(LEXER).yy.c
	@rm $(LEXER).yy.c $(PARSER).tab.c $(PARSER).tab.h

example:
	@./$(PARSER) example.cq --dump
	@cat tree_dump.out
	@cat symbol_table_dump.out

test:
	@printf "Running tests...\n"; \

	@for dir in $(TEST_DIR)/*/; do \
  		if [ -d "$$dir" ]; then \
  			for file in "$$dir"*; do \
  				if [ -f "$$file" ]; then \
					./$(PARSER) $$file; \
					if [ $$? -ne 0 ]; then \
						echo "$(PARSER) returned 1 for file $$(basename $$file)"; \
					exit 1; \
					fi; \
				fi; \
			done; \
			printf "|- %s passed.\n" "$$dir"; \
		fi; \
	done; \

clean:
	@rm -f $(PARSER) $(PARSER).output symtab_dump.out $(PARSER).tab.c $(PARSER).tab.h $(LEXER).yy.c
