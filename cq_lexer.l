%option yylineno
%option noyywrap

%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "cq_parser.tab.h"
extern int yylineno;

bool strtob(const char *str, const char **endptr);

%}

%x COMMENT

%{ /* tokens */
%}
ID				        [a-zA-Z_][a-zA-Z0-9_]*
BCONST                  "false"|"true"
FCONST				    "-"?[0-9]+"."[0-9]+
ICONST				    "-"?[0-9]+
UCONST				    [0-9]+

%%

%{ /* increase line count */
%}
"\n"				    {}
[ \t\r\f]+			    /* ignore whitespace */

%{ /* comments */
%}

"/*"				    { BEGIN(COMMENT); }
<COMMENT>"*/"			{ BEGIN(INITIAL); }
<COMMENT>[^\*\n]*		{ /* Ignore anything that is not a '*' or '\n' */ }
<COMMENT>\*+[^/\n]*	    { /* Ignore any sequence of '*' not followed by '/' or '\n' */ }
<COMMENT>\n			    {}
"//"[^\n]*\n			{}

%{ /* types */
%}
bool				    { return BOOL; }
float				    { return FLOAT; }
int				        { return INT; }
unsigned			    { return UNSIGNED; }
void				    { return VOID; }

%{ /* type qualifier */
%}
const				    { return CONST; }
quantum				    { return QUANTUM; }

%{ /* classical control flow */
%}
break				    { return BREAK; }
continue			    { return CONTINUE; }
do				        { return DO; }
for				        { return FOR; }
return				    { return RETURN; }
while				    { return WHILE; }

%{ /* hybrid control flow */
%}
case				    { return CASE; }
default				    { return DEFAULT; }
else				    { return ELSE; }
if				        { return IF; }
switch				    { return SWITCH; }

%{ /* measure */
%}
measure				    { return MEASURE; }

%{ /* phase */
%}
phase				    { return PHASE; }

%{ /* arithmetic operations */
%}
"+"				        { return ADD; }
"/"				        { return DIV; }
"%"				        { return MOD; }
"*"				        { return MUL; }
"-"				        { return SUB; }

%{ /* bitwise operations */
%}
"&"				        { return BIT_AND; }
"<<"				    { return BIT_LSHIFT; }
"|"				        { return BIT_OR; }
">>"				    { return BIT_RSHIFT; }
"^"				        { return BIT_XOR; }

%{ /* boolean operations */
%}
"&&"				    { return AND; }
"=="				    { return EQ; }
">"				        { return GE; }
">="				    { return GEQ; }
"<"				        { return LE; }
"<="				    { return LEQ; }
"!="				    { return NEQ; }
"!"				        { return NOT; }
"~"				        { return NOT_OP; }
"||"				    { return OR; }
"^^"				    { return XOR; }

%{ /* assignments */
%}
"="				        { return ASSIGN; }
"+="				    { return ASSIGN_ADD; }
"&="				    { return ASSIGN_AND; }
"/="				    { return ASSIGN_DIV; }
"<<="				    { return ASSIGN_LSHIFT; }
"%="				    { return ASSIGN_MOD; }
"*="				    { return ASSIGN_MUL; }
"|="				    { return ASSIGN_OR; }
">>="				    { return ASSIGN_RSHIFT; }
"-="				    { return ASSIGN_SUB; }
"^="				    { return ASSIGN_XOR; }

%{ /* delimiters */
%}
":"				        { return COLON; }
","				        { return COMMA; }
"{"				        { return LBRACE; }
"["				        { return LBRACKET; }
"("				        { return LPAREN; }
"}"				        { return RBRACE; }
"]"				        { return RBRACKET; }
")"				        { return RPAREN; }
";"				        { return SEMICOLON; }

%{ /* token actions */
%}

{BCONST}                { yylval.bconst = strtob(yytext, NULL); return BCONST; }
{ID}				    { insert(yytext, strlen(yytext), UNDEFINED_T, yylineno);  yylval.symtab_item = lookup(yytext); return ID; }
{UCONST}			    { yylval.uconst = (unsigned) strtoul(yytext, NULL, 10); return UCONST; }
{ICONST}			    { yylval.iconst = (int) strtol(yytext, NULL, 10); return ICONST; }
{FCONST}			    { yylval.fconst = strtod(yytext, NULL); return FCONST; }

%{ /* match not found */
%}
.				        { fprintf(stderr, "%u: error: %s \n", yylineno, yytext); }

%%

bool strtob(const char *str, const char **endptr) {
    size_t len = strlen(str);
    if (len < 4) {
        if (endptr != NULL) {
            *endptr = str;
        }
        return false;
    } else if (len == 4) {
        if (strncmp(str, "true", 4) == 0) {
            if (endptr != NULL) {
                *endptr = str + 4;
            }
            return true;
        } else {
            if (endptr != NULL) {
                *endptr = str;
            }
            return false;
        }
    } else {
        if (strncmp(str, "true", 4) == 0) {
            if (endptr != NULL) {
                *endptr = str + 4;
            }
            return true;
        } else if (strncmp(str, "false", 5) == 0) {
            if (endptr != NULL) {
                *endptr = str + 5;
            }
            return false;
        } else {
            if (endptr != NULL) {
                *endptr = str;
            }
            return false;
        }
    }
}
