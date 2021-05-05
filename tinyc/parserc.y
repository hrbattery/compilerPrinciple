%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char*);
#define YYSTYPE char *
%}

%token INT MAIN VOID BREAK DO
%token ELSE IF WHILE RETURN READ
%token WRITE LBRACE RBRACE LSQUARE RSQUARE LPAR
%token RPAR SEMI PLUS MINUS MUL_OP
%token DIV_OP AND_OP OR_OP NOT_OP ASSIGN
%token LT GT SHL_OP SHR_OP EQ
%token NOTEQ LTEQ GTEQ ANDAND OROR
%token COMMA POINT INT_NUM IDENTIFIER
// %left '+' '-'
// %left '*' '/'
// %right U_neg

%%
program : var_declarations statements
var_declarations : var_declarations var_declaration | /* empty */
var_declaration : INT declaration_list SEMI
declaration_list : declaration_list COMMA declaration | declaration
declaration : IDENTIFIER ASSIGN INT_NUM | IDENTIFIER LSQUARE INT_NUM RSQUARE | IDENTIFIER
code_block : statement | LBRACE statements RBRACE
statements : statements statement | statement
statement : assign_statement SEMI | control_statement | read_write_statement SEMI | SEMI
control_statement : if_statement | while_statement | do_while_statement SEMI | return_statement SEMI
read_write_statement : read_statement | write_statement
assign_statement : IDENTIFIER LSQUARE exp RSQUARE ASSIGN exp | IDENTIFIER ASSIGN exp
if_statement : if_stmt | if_stmt ELSE code_block
if_stmt : IF LPAR exp RPAR code_block
while_statement : WHILE LPAR exp RPAR code_block
do_while_statement : DO code_block WHILE LPAR exp RPAR
return_statement : RETURN
read_statement : READ LPAR IDENTIFIER RPAR
write_statement : WRITE LPAR exp RPAR
exp: exp2 | exp OROR exp2
exp2: exp3 | exp2 ANDAND exp3
exp3: exp4 | exp3 OR_OP exp4
exp4: exp5 | exp4 AND_OP exp5
exp5: exp6 | exp5 EQ exp6 | exp5 NOTEQ exp6
exp6: exp7 | exp6 LT exp7 | exp6 GT exp7 | exp6 LTEQ exp7 | exp6 GTEQ exp7
exp7: exp8 | exp7 SHL_OP exp8 | exp7 SHR_OP exp8
exp8: exp9 | exp8 PLUS exp9 | exp8 MINUS exp9
exp9: exp10 | exp9 MUL_OP exp10 | exp9 DIV_OP exp10
exp10 : exp11 | MINUS exp11 | NOT_OP exp11
exp11: INT_NUM | IDENTIFIER | IDENTIFIER LSQUARE exp RSQUARE | LPAR exp RPAR


%%

int main () {
    return yyparse();
}