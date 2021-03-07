%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char * msg);
#define YYSTYPE char *
%}

%token BEGINSYM END READ WRITE ID
%token LPAREN RPAREN SEMICOLON COMMA
%token ASSIGNOP PLUOP MINUSOP SCANEOF
%token INTLITERAL

// %type <str_value> Expn StmtList Stmt IdList ExpnList Primary

%left ASSIGNOP
%left PLUOP MINUSOP

%start SystemGoal

// %union {
//     int num;
//     char* str_value;
// }

%%

Program:        BEGINSYM StmtList END                   { printf("program \t%s%s%s\n", $1, $2, $3); }
        ;

StmtList:       Stmt SEMICOLON
        |       Stmt StmtList SEMICOLON

Stmt:           ID ASSIGNOP Expn                        { printf("assign stmt\t%s%s%s\n", $1, $2, $3); }
        |       READ LPAREN IdList RPAREN               { printf("read stmt\t%s%s%s%s\n", $1, $2, $3, $4); }
        |       WRITE LPAREN ExpnList RPAREN            { printf("write stmt\t%s%s%s%s\n", $1, $2, $3, $4); }
        ;

IdList:         ID
        |       ID COMMA IdList
        ;

ExpnList:       Expn
        |       Expn COMMA ExpnList
        ;

Expn:           Primary
        |       Primary PLUOP Expn                      { printf("plus expn\t%s%s%s\n", $1, $2, $3); }
        |       Primary MINUSOP Expn                    { printf("minus expns\n"); }
        ;

Primary:        LPAREN Expn RPAREN                      { printf("paren expn\t%s%s%s\n", $1, $2, $3); }
        |       ID                                      { printf("id\t%s\n", $1); }
        |       INTLITERAL                              { printf("int\t%d\n", atoi($1)); }
        ;

SystemGoal:     Program SCANEOF                         { printf("SystemGoal\n"); }
        ;
%%

int main(int argc, char* argv[]) {
    return yyparse();
}

void yyerror(const char * msg) {
    extern int yydebug;
    yydebug = 1;
    printf("error: %s\n", msg);
};
