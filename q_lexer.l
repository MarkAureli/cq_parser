%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_parser.tab.h"
%}

/* Define tokens */
IDENTIFIER	[a-zA-Z_][a-zA-Z0-9_]*
NUMBER		[0-9]+
WHITESPACE	[ \t\n]+

%%

true		{ return TRUE; }
false		{ return FALSE; }
void		{ return VOID; }
bool		{ return BOOL; }
int		{ return INT; }
measure		{ return MEASURE; }
quantum		{ return QUANTUM; }
const		{ return CONST; }
if		{ return IF; }
else		{ return ELSE; }
for		{ return FOR; }
return		{ return RETURN; }

"+"		{ return ADD; }
"-"		{ return SUB; }
"*"		{ return MUL; }
"/"		{ return DIV; }
"%"		{ return MOD; }
"="		{ return ASSIGN; }
"+="		{ return ADD_ASSIGN; }
"-="		{ return SUB_ASSIGN; }
"*="		{ return MUL_ASSIGN; }
"/="		{ return DIV_ASSIGN; }
"%="		{ return MOD_ASSIGN; }
"&&"		{ return AND; }
"||"		{ return OR; }
"=="		{ return EQ; }
"!="		{ return NEQ; }
"<="		{ return LEQ; }
">="		{ return GEQ; }
"<"		{ return LE; }
">"		{ return GE; }
"!"		{ return NOT; }
"~"		{ return NOT_OP; }
"&"		{ return AND_OP; }
"&="		{ return AND_ASSIGN; }
"|"		{ return OR_OP; }
"|="		{ return OR_ASSIGN; }
">>"		{ return BIT_RSHIFT; }
"<<"		{ return BIT_LSHIFT; }

"("		{ return LPAREN; }
")"		{ return RPAREN; }
"{"		{ return LBRACE; }
"}"		{ return RBRACE; }
"["		{ return LBRACKET; }
"]"		{ return RBRACKET; }
";"		{ return SEMICOLON; }

{IDENTIFIER}	{ yylval.str = strdup(yytext); return IDENTIFIER; }
{NUMBER}	{ yylval.num = atoi(yytext); return NUMBER; }
{WHITESPACE}	{ /* Ignore whitespace */ }

.		{ printf("Unknown character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

