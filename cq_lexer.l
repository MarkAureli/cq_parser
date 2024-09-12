%option yylineno
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cq_parser.tab.h"

unsigned line_num = 1;
%}

%x COMMENT

%{ /* tokens */
%}
ID				[a-zA-Z_][a-zA-Z0-9_]*
ICONST				[0-9]+
FCONST				[0-9]+"."[0-9]+	

%%

%{ /* increase line count */
%}
"\n"				{ ++line_num; }
[ \t\r\f]+			/* ignore whitespace */

%{ /* comments */
%}

"/*"				{ BEGIN(COMMENT); }
<COMMENT>"*/"			{ BEGIN(INITIAL); }
<COMMENT>[^\*\n]*		{ /* Ignore anything that is not a '*' or '\n' */ }
<COMMENT>\*+[^/\n]*		{ /* Ignore any sequence of '*' not followed by '/' or '\n' */ }
<COMMENT>\n			{ line_num++; }
"//"[^\n]*\n			{ line_num++; }

%{ /* types */
%}
bool				{ return BOOL; }
float				{ return FLOAT; }
int				{ return INT; }
unsigned			{ return UNSIGNED; }
void				{ return VOID; }

%{ /* type qualifier */
%}
const				{ return CONST; }
quantum				{ return QUANTUM; }

%{ /* truth values */
%}
true				{ return TRUE; }
false				{ return FALSE; }

%{ /* classical control flow */
%}
break				{ return BREAK; }
continue			{ return CONTINUE; }
do				{ return DO; }
for				{ return FOR; }
return				{ return RETURN; }
while				{ return WHILE; }

%{ /* hybrid control flow */
%}
case				{ return CASE; }
default				{ return DEFAULT; }
else				{ return ELSE; }
if				{ return IF; }
switch				{ return SWITCH; }

%{ /* measure */
%}
measure				{ return MEASURE; }

%{ /* phase */
%}
phase				{ return PHASE; }

%{ /* arithmetic operations */
%}
"+"				{ return ADD; }
"/"				{ return DIV; }
"%"				{ return MOD; }
"*"				{ return MUL; }
"-"				{ return SUB; }

%{ /* bitwise operations */
%}
"&"				{ return BIT_AND; }
"<<"				{ return BIT_LSHIFT; }
"|"				{ return BIT_OR; }
">>"				{ return BIT_RSHIFT; }
"^"				{ return BIT_XOR; }

%{ /* boolean operations */
%}
"&&"				{ return AND; }
"=="				{ return EQ; }
">"				{ return GE; }
">="				{ return GEQ; }
"<"				{ return LE; }
"<="				{ return LEQ; }
"!="				{ return NEQ; }
"!"				{ return NOT; }
"~"				{ return NOT_OP; }
"||"				{ return OR; }
"^^"				{ return XOR; }

%{ /* assignments */
%}
"="				{ return ASSIGN; }
"+="				{ return ASSIGN_ADD; }
"&="				{ return ASSIGN_AND; }
"/="				{ return ASSIGN_DIV; }
"<<="				{ return ASSIGN_LSHIFT; }
"%="				{ return ASSIGN_MOD; }
"*="				{ return ASSIGN_MUL; }
"|="				{ return ASSIGN_OR; }
">>="				{ return ASSIGN_RSHIFT; }
"-="				{ return ASSIGN_SUB; }
"^="				{ return ASSIGN_XOR; }

%{ /* delimiters */
%}
":"				{ return COLON; }
","				{ return COMMA; }
"{"				{ return LBRACE; }
"["				{ return LBRACKET; }
"("				{ return LPAREN; }
"}"				{ return RBRACE; }
"]"				{ return RBRACKET; }
")"				{ return RPAREN; }
";"				{ return SEMICOLON; }

%{ /* token actions */
%}
{ID}				{ yylval.str = strdup(yytext); return ID; }
{ICONST}			{ yylval.iconst = atoi(yytext); return ICONST; }
{FCONST}			{ yylval.fconst = atoi(yytext); return FCONST; }

%{ /* match not found */
%}
.				{ fprintf(stderr, "%u: error: %s \n", line_num, yytext); }

%%

