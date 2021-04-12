#include "LALR.h"
using namespace std;

Symbol production::getLeft() {
    return left;
}

Symbol production::getFirstRight() {
    return right[0];
}

void LALR::initCFG() {
    add(production(program, {var_decls, stmts}));

    add(production(var_decls, {var_decls, var_decl}));
    add(production(var_decls, {EMPTY}));

    add(production(var_decl, {INT, decl_list, SEMI}));

    add(production(decl_list, {decl_list, COMMA, decl}));
    add(production(decl_list, {decl}));

    add(production(decl, {ID, ASSIGN, INT_NUM}));
    add(production(decl, {ID, LSQUARE, INT_NUM, RSQUARE}));
    add(production(decl, {ID}));

    add(production(code_block, {stmt}));
    add(production(code_block, {LBRACE, stmts, RBRACE}));

    add(production(stmts, {stmts, stmt}));
    add(production(stmts, {stmt}));

    add(production(stmt, {assign_stmt, SEMI}));
    add(production(stmt, {ctrl_stmt}));
    add(production(stmt, {rw_stmt, SEMI}));
    add(production(stmt, {SEMI}));
    
    add(production(ctrl_stmt, {if_statement}));
    add(production(ctrl_stmt, {while_stmt}));
    add(production(ctrl_stmt, {do_while_stmt, SEMI}));
    add(production(ctrl_stmt, {ret_stmt, SEMI}));

    add(production(rw_stmt, {r_stmt}));
    add(production(rw_stmt, {w_stmt}));

    add(production(assign_stmt, {ID, LSQUARE, expr, RSQUARE, ASSIGN, expr}));
    add(production(assign_stmt, {ID, ASSIGN, expr}));

    add(production(if_statement, {if_stmt}));
    add(production(if_statement, {if_stmt, ELSE, code_block}));
    
    add(production(if_stmt, {IF, LPAR, expr, RPAR, code_block}));

    add(production(while_stmt, {WHILE, LPAR, expr, RPAR, code_block}));

    add(production(do_while_stmt, {DO, code_block, WHILE, LPAR, expr, RPAR}));

    add(production(ret_stmt, {RETURN}));

    add(production(r_stmt, {READ, LPAR, ID, RPAR}));

    add(production(w_stmt, {WRITE, LPAR, expr, RPAR}));

    add(production(expr, {INT_NUM}));
    add(production(expr, {ID}));
    add(production(expr, {ID, LSQUARE, expr, RSQUARE}));
    add(production(expr, {NOT_OP, expr}));
    add(production(expr, {expr, AND_OP, expr}));
    add(production(expr, {expr, OR_OP, expr}));
    add(production(expr, {expr, PLUS, expr}));
    add(production(expr, {expr, MINUS, expr}));
    add(production(expr, {expr, MUL_OP, expr}));
    add(production(expr, {expr, DIV_OP, expr}));
    add(production(expr, {expr, LT, expr}));
    add(production(expr, {expr, GT, expr}));
    add(production(expr, {expr, EQ, expr}));
    add(production(expr, {expr, NOTEQ, expr}));
    add(production(expr, {expr, LTEQ, expr}));
    add(production(expr, {expr, GTEQ, expr}));
    add(production(expr, {expr, SHL_OP, expr}));
    add(production(expr, {expr, SHR_OP, expr}));
    add(production(expr, {expr, ANDAND, expr}));
    add(production(expr, {expr, OROR, expr}));
    add(production(expr, {LPAR, expr, RPAR}));
    add(production(expr, {MINUS, expr}));
    first();
    follow();
}
// first(): init first set
void LALR::first() {
    // First, init a begin status of first set
    for (auto &p: G) {
        Symbol left = p.getLeft();
        Symbol firstRight = p.getFirstRight();
        FIRST[left].insert(firstRight);
    }
}
void LALR::build() {

}

void LALR::add(const production &prod) {
    G.push_back(prod);
}

void LALR::run(FILE* fp) {
    initCFG();
    build();
    tokenList = scan(fp);
    parser();
}

int main(int argc, char* argv[]) {
    FILE* fp;
    if (argc > 1) {
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("Unable to open file.");
            exit(0);
        }
    }
    printf("%s", "Result: \n");

    LALR lalr;
    lalr.run(fp);

    fclose(fp);
    fp = NULL;
    return 0;
}