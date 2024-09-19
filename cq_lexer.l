%option yylineno
%option noyywrap

%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
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

%{ /* logical operations */
%}
"&&"				    { yylval.logical_op = LAND_OP; return LAND; }
"||"				    { yylval.logical_op = LOR_OP;  return LOR;  }
"^^"				    { yylval.logical_op = LXOR_OP; return LXOR; }

%{ /* comparison operations */
%}
">"				        { yylval.comparison_op = GE_OP;  return GE;  }
">="				    { yylval.comparison_op = GEQ_OP; return GEQ; }
"<"				        { yylval.comparison_op = LE_OP;  return LE;  }
"<="				    { yylval.comparison_op = LEQ_OP; return LEQ; }

%{ /* equality operations */
%}
"=="				    { yylval.equality_op = EQ_OP;  return EQ;  }
"!="				    { yylval.equality_op = NEQ_OP; return NEQ; }

%{ /* not operation */
%}
"!"                     { return NOT; }

%{ /* integer operations */
%}
"+"				        { yylval.integer_op = ADD_OP; return ADD; }
"&"				        { yylval.integer_op = AND_OP; return AND; }
"/"				        { yylval.integer_op = DIV_OP; return DIV; }
"%"				        { yylval.integer_op = MOD_OP; return MOD; }
"*"				        { yylval.integer_op = MUL_OP; return MUL; }
"|"				        { yylval.integer_op = OR_OP;  return OR;  }
"-"				        { yylval.integer_op = SUB_OP; return SUB; }
"^"				        { yylval.integer_op = XOR_OP; return XOR; }

%{ /* invert operation */
%}
"~"                     { return INV; }

%{ /* assignments */
%}
"="				        { yylval.assign_op = ASSIGN_OP;     return ASSIGN;     }
"|="				    { yylval.assign_op = ASSIGN_OR_OP;  return ASSIGN_OR;  }
"^="				    { yylval.assign_op = ASSIGN_XOR_OP; return ASSIGN_XOR; }
"&="				    { yylval.assign_op = ASSIGN_AND_OP; return ASSIGN_AND; }
"+="				    { yylval.assign_op = ASSIGN_ADD_OP; return ASSIGN_ADD; }
"-="				    { yylval.assign_op = ASSIGN_SUB_OP; return ASSIGN_SUB; }
"*="				    { yylval.assign_op = ASSIGN_MUL_OP; return ASSIGN_MUL; }
"/="				    { yylval.assign_op = ASSIGN_DIV_OP; return ASSIGN_DIV; }
"%="				    { yylval.assign_op = ASSIGN_MOD_OP; return ASSIGN_MOD; }

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

{BCONST}                { yylval.value.bval = strtob(yytext, NULL); return BCONST; }
{ID}				    { yylval.name = malloc(yyleng * sizeof (char)); strcpy(yylval.name, yytext); return ID; }
{ICONST}			    { yylval.value.ival = (int) strtol(yytext, NULL, 10); return ICONST; }

%{ /* match not found */
%}
.				        { fprintf(stderr, "Parsing failed in line %u: unknown symbol %s \n", yylineno, yytext); exit(1); }

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
