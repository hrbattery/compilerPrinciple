%{
#include <ctype.h>
%}
%option noyywrap
UPPER [A-Z]
%%
{UPPER} {
    printf("%d",yyleng);
}
%%
int main(int argc, char **argv) {
    return yylex();
}
