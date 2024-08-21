%{
#include <stdio.h>
#include <stdbool.h>
%}

%x COMMENT

/* Declare tokens */
IDENTIFIER    [a-zA-Z_][a-zA-Z0-9_]*
NUMBER        [0-9]+
WHITESPACE    [ \t\n]+

%%

%{ /* comments */
%}

"/*"		{ BEGIN(COMMENT); }
<COMMENT>"*/"	{ BEGIN(INITIAL); }
<COMMENT>[^\*]*	{ }
<COMMENT>\*+([^*/][^*\n]*)? { }

bool		{ printf("TYPE: %s\n", yytext); }
int		{ printf("TYPE: %s\n", yytext); }
const		{ printf("DECORATOR: %s\n", yytext); }
quantum		{ printf("DECORATOR: %s\n", yytext); }
true		{ printf("%s\n", yytext); }
false		{ printf("%s\n", yytext); }
{IDENTIFIER}	{ printf("IDENTIFIER: %s\n", yytext); }
{NUMBER}	{ printf("NUMBER: %s\n", yytext); }
{WHITESPACE}	{ /* Ignore whitespace */ }
"+"		{ printf("ADDITION OPERATOR: %s\n", yytext); }
"-"		{ printf("SUBTRACTION OPERATOR: %s\n", yytext); }
"="		{ printf("ASSIGNMENT OPERATOR: %s\n", yytext); }

.		{ printf("Unknown character: %s\n", yytext); }

%%

int main(int argc, char **argv)
{
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

