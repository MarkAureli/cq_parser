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
%token VOID BOOL INT QUANTUM CONST
%token TRUE FALSE
%token ADD SUB MUL DIV MOD
%token ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token AND OR EQ NEQ LEQ GEQ LE GE
%token NOT NOT_OP AND_OP AND_ASSIGN OR_OP OR_ASSIGN
%token BIT_RSHIFT BIT_LSHIFT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON
%token IF ELSE FOR RETURN MEASURE

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
