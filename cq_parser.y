%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(const char *s);
int success = 1;
extern FILE *yyin;

%}

/* Union to define yylval's types */
%union {
    char *str;
    int int_const;
    double float_const;
}

%token <str> IDENTIFIER
%token <int_const> INT_CONST
%token <float_const> FLOAT_CONST
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
%token COLON COMMA LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON
%left ADD SUB
%left MUL DIV MOD
%nonassoc "then"
%nonassoc ELSE
%define parse.error verbose
%start program

%%

program:
	declaration_list
	;

declaration_list:
	declaration declaration_list
	| declaration
	;

declaration:
	variable_declaration
	| variable_definition
	| function_definition
	;

function_definition:
	QUANTUM type_specifier declarator LPAREN parameter_list RPAREN statement
	| type_specifier declarator IDENTIFIER LPAREN parameter_list RPAREN statement
	;

parameter_list:
	parameter_list COMMA parameter
	| parameter
	| /* empty */
	;

parameter:
	QUANTUM type_specifier declarator
	| type_specifier declarator
	;

variable_declaration:
	QUANTUM type_specifier declarator SEMICOLON
	| type_specifier declarator SEMICOLON
	;

variable_definition:
	QUANTUM type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| QUANTUM type_specifier declarator ASSIGN LBRACKET initializer_list RBRACKET SEMICOLON
	| CONST type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| CONST type_specifier declarator ASSIGN LBRACE initializer_list RBRACE SEMICOLON
	| type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| type_specifier declarator ASSIGN LBRACE initializer_list RBRACE SEMICOLON

declarator:
	IDENTIFIER
	| declarator LBRACKET logical_or_expr RBRACKET
	| declarator LBRACKET RBRACKET

initializer_list:
	initializer_list COMMA logical_or_expr
	| logical_or_expr
	;

type_specifier:
	BOOL
	| FLOAT
	| INT
	| UNSIGNED
	| VOID
	;

statement:
	case_statement
	| expr_statement
	| compound_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

case_statement:
	CASE logical_or_expr COLON statement
	| DEFAULT COLON statement
	;

expr_statement:
	expr SEMICOLON
	| SEMICOLON
	;

compound_statement:
	LBRACE declaration_list statement_list RBRACE
	| LBRACE statement_list RBRACE
	| LBRACE declaration_list RBRACE
	| LBRACE RBRACE
	;

statement_list:
	statement
	| statement_list statement
	;

selection_statement:
	IF LPAREN logical_or_expr RPAREN statement	%prec "then"
	| IF LPAREN logical_or_expr RPAREN statement ELSE statement
	| SWITCH LPAREN logical_or_expr RPAREN statement
	;

iteration_statement:
	DO statement WHILE LPAREN expr RPAREN SEMICOLON
	| WHILE LPAREN expr RPAREN statement
	| FOR LPAREN expr_statement expr_statement expr RPAREN statement
	;

jump_statement:
	CONTINUE SEMICOLON
	| BREAK SEMICOLON
	| RETURN logical_or_expr SEMICOLON
	| RETURN SEMICOLON
	;

expr:
	logical_or_expr
	| unary_expr assignment_operator expr
	;

assignment_operator:
	ASSIGN_AND
	| ASSIGN_OR
	| ASSIGN_XOR
	| ASSIGN_LSHIFT
	| ASSIGN_RSHIFT
	| ASSIGN_MUL
	| ASSIGN_DIV
	| ASSIGN_MOD
	| ASSIGN_ADD
	| ASSIGN_SUB
	| ASSIGN
	;

logical_or_expr:
	logical_xor_expr
	| logical_or_expr OR logical_xor_expr
	;

logical_xor_expr:
	logical_and_expr
	| logical_xor_expr XOR logical_and_expr
	;

logical_and_expr:
	bit_or_expr
	| logical_and_expr AND bit_or_expr
	;

bit_or_expr:
	bit_xor_expr
	| bit_or_expr BIT_OR bit_xor_expr
	;

bit_xor_expr:
	bit_and_expr
	| bit_xor_expr BIT_XOR bit_and_expr
	;

bit_and_expr:
	equality_expr
	| bit_and_expr BIT_AND equality_expr
	;

equality_expr:
	relational_expr
	| equality_expr EQ relational_expr
	| equality_expr NEQ relational_expr
	;

relational_expr:
	shift_expr
	| relational_expr LE shift_expr
	| relational_expr GE shift_expr
	| relational_expr LEQ shift_expr
	| relational_expr GEQ shift_expr
	;

shift_expr:
	add_expr
	| shift_expr BIT_LSHIFT add_expr
	| shift_expr BIT_RSHIFT add_expr
	;

add_expr:
	mul_expr
	| add_expr ADD mul_expr
	| add_expr SUB mul_expr
	;

mul_expr:
	unary_expr
	| mul_expr MUL unary_expr
	| mul_expr DIV unary_expr
	| mul_expr MOD unary_expr
	;

unary_expr:
	postfix_expr
	| unary_op unary_expr
	| MEASURE LPAREN postfix_expr RPAREN
	| PHASE LPAREN postfix_expr RPAREN
	;

unary_op:
	BIT_AND
	| MUL
	| ADD
	| SUB
	| NOT_OP
	| NOT
	;

postfix_expr:
	primary_expr
	| postfix_expr LBRACKET logical_or_expr RBRACKET
	| postfix_expr LPAREN argument_expr_list RPAREN
	| postfix_expr LPAREN RPAREN
	;

primary_expr:
	IDENTIFIER
	| consts
	| LPAREN logical_or_expr RPAREN
	;

argument_expr_list:
	logical_or_expr
	| argument_expr_list COMMA logical_or_expr
	;

consts:
	INT_CONST
	| FLOAT_CONST
	| FALSE
	| TRUE
	;

%%

int yyerror(const char *msg) {
    extern int yylineno;
    fprintf(stderr, "Parsing failed in line %d: %s\n", yylineno, msg);
    success = 0;
    return 0;
}

int main(int argc, char **argv) {
#ifdef YYDEBUG
    yydebug = 1;
#endif

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    yyparse();
    if (success == 1) {
        printf("Parsing successful\n");
	return 0;
    } else {
        return 1;	    
    }
}
