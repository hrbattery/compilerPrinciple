%{
#include <stdio.h>
#include <stdlib.h>
// #include <unordered_map>
void yyerror(const char * msg);
#define YYSTYPE char *
int yylex ();

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

StmtList:       Stmt
        |       Stmt StmtList

Stmt:           ID ASSIGNOP Expn SEMICOLON                       { printf("assign stmt\t%s%s%s\n", $1, $2, $3); }
        |       READ LPAREN IdList RPAREN SEMICOLON              { printf("read stmt\t%s%s%s%s\n", $1, $2, $3, $4); }
        |       WRITE LPAREN ExpnList RPAREN SEMICOLON           { printf("write stmt\t%s%s%s%s\n", $1, $2, $3, $4); }
        ;

IdList:         ID                                      { printf("idList's id\t%s\n", $1); }
        |       ID COMMA IdList
        ;

ExpnList:       Expn
        |       Expn COMMA ExpnList
        ;

Expn:           Primary
        |       Primary PLUOP Expn                      { printf("plus expn\t%s%s%s\n", $1, $2, $3); $$=$3; }
        |       Primary MINUSOP Expn                    { printf("minus expn\t%s%s%s\n", $1, $2, $3); }
        ;

Primary:        LPAREN Expn RPAREN                      { printf("paren expn\t%s%s%s\n", $1, $2, $3); }
        |       ID                                      { printf("id\t%s\n", $1); }
        |       INTLITERAL                              { printf("int\t%d\n", atoi($1)); }
        ;

SystemGoal:     Program SCANEOF                         { printf("SystemGoal\n"); }
        ;
%%

int main(int argc, char* argv[]) {
    extern int yydebug;
    yydebug = 1;
    return yyparse();
}

void yyerror(const char * msg) {
    printf("error: %s\n", msg);
};


