%{
#include <stdio.h>
int yylex();
void yyerror(const char* msg) {}
%}

%token T_NUM

%left '+' '-'
%left '*' '/'

%%

S   :   S E '\n'    { printf("ans=%d\n", $2); }
    |   /* empty */ { /* empty */ }
    ;

E   :   E '+' E     { $$ = $1 + $3; }
    |   E '-' E     { $$ = $1 - $3; }
    |   E '*' E     { $$ = $1 * $3; }
    |   E '/' E     { $$ = $1 / $3; }
    |   T_NUM       { $$ = $1; }
    |   '(' E ')'   { $$ = $2; }
    ;
%%

int main(int argc, char **argv) {
    return yyparse();
}