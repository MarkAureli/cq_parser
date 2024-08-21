%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;

%}

/* Union to define yylval's types */
%union {
    char *str;
    int num;
}

%token <str> IDENTIFIER
%token <num> NUMBER
%token BOOL FLOAT INT UNSIGNED VOID
%token CONST QUANTUM
%token BREAK CONTINUE DO FOR RETURN WHILE
%token CASE DEFAULT ELSE IF SWITCH
%token TRUE FALSE
%token MEASURE
%token PHASE
%token ADD DIV MOD MUL SUB
%token BIT_AND BIT_LSHIFT BIT_OR BIT_RSHIFT BIT_XOR
%token AND EQ GE GEQ LE LEQ NEQ NOT NOT_OP OR XOR
%token ASSIGN ASSIGN_ADD ASSIGN_AND ASSIGN_DIV ASSIGN_LSHIFT ASSIGN_MOD ASSIGN_MUL ASSIGN_OR ASSIGN_RSHIFT ASSIGN_SUB ASSIGN_XOR
%token LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON

%%

program:
    statement_list
    ;

statement_list:
	statement_list statement
	| statement
	;

statement:
	declaration
	| QUANTUM declaration
	| assignment
	;

declaration:
	BOOL IDENTIFIER SEMICOLON
	{ printf("bool %s;\n", $2); free($2); }
	| INT IDENTIFIER SEMICOLON
	{ printf("int %s;\n", $2); free($2); }
	| BOOL IDENTIFIER ASSIGN TRUE SEMICOLON
	{ printf("bool %s = true;\n", $2); free($2); }
	| BOOL IDENTIFIER ASSIGN FALSE SEMICOLON
	{ printf("bool %s = false;\n", $2); free($2); }
	| INT IDENTIFIER ASSIGN NUMBER SEMICOLON
	{ printf("int %s = %d;\n", $2, $4); free($2); }
	;

assignment:
    IDENTIFIER ASSIGN NUMBER
    { printf("Assignment: %s = %d\n", $1, $3); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    yyparse();
    return 0;
}
