#include "LALR.h"
using namespace std;

Symbol production::getLeft() {
    return left;
}

Symbol production::getFirstRight() {
    if (left == right[0]) {
        return right[1];
    }
    return right[0];
}

Symbol production::getSecondRight() {
    if (right.size() >= 2) {
        return right[1];
    } else {
        return EMPTY;
    }
}

string production::leftStr() {
    return token_strs[left];
}

string production::rightStr() {
    string ret;
    for (unsigned i = 0; i < right.size(); i++) {
        ret += token_strs[right[i]];
        if (i != right.size()-1) {
            ret += " ";
        }
    }
    return ret;
}

void state::add(const item &conf) {
    items.push_back(conf);
}

Symbol item::getDotBackSymbol() {
    if (dotPos >= prod.right.size()-1) {
        return EMPTY;
    } else {
        return prod.right[dotPos+1];
    }
}

void LALR::showStatusStack() {
    for(vector<int>::iterator it = status.begin(); it != status.end(); ++it) {
        printf(" %d ", *it);
    }
}

void LALR::showParseStack() {
    for(vector<token>::iterator it = parse.begin(); it != parse.end(); ++it)
        printf(" %d ", (*it).tokentype);
}

void LALR::initCFG() {
//    add(production(start, {program, END}));
    add(production(start, {program}));

    add(production(program, {var_decls, stmts}));
    add(production(program, {stmts}));

    add(production(var_decls, {var_decls, var_decl}));
    add(production(var_decls, {var_decl}));
//    add(production(var_decls, {EMPTY}));

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

    add(production(expr, {expr2}));
    add(production(expr, {expr, OROR, expr2}));
    
    add(production(expr2, {expr3}));
    add(production(expr2, {expr2, ANDAND, expr3}));

    add(production(expr3, {expr4}));
    add(production(expr3, {expr3, OR_OP, expr4}));

    add(production(expr4, {expr5}));
    add(production(expr4, {expr4, AND_OP, expr5}));

    add(production(expr5, {expr6}));
    add(production(expr5, {expr5, EQ, expr6}));
    add(production(expr5, {expr5, NOTEQ, expr6}));

    add(production(expr6, {expr7}));
    add(production(expr6, {expr6, LT, expr7}));
    add(production(expr6, {expr6, GT, expr7}));
    add(production(expr6, {expr6, LTEQ, expr7}));
    add(production(expr6, {expr6, GTEQ, expr7}));

    add(production(expr7, {expr8}));
    add(production(expr7, {expr7, SHL_OP, expr8}));
    add(production(expr7, {expr7, SHR_OP, expr8}));

    add(production(expr8, {expr9}));
    add(production(expr8, {expr8, PLUS, expr9}));
    add(production(expr8, {expr8, MINUS, expr9}));

    add(production(expr9, {expr10}));
    add(production(expr9, {expr9, MUL_OP, expr10}));
    add(production(expr9, {expr9, DIV_OP, expr10}));

    add(production(expr10, {expr11}));
    add(production(expr10, {MINUS, expr11}));
    add(production(expr10, {NOT_OP, expr11}));

    add(production(expr11, {INT_NUM}));
    add(production(expr11, {ID}));
    add(production(expr11, {ID, LSQUARE, expr, RSQUARE}));
    add(production(expr11, {LPAR, expr, RPAR}));
    
    first();
    // follow();
}
// first(): init first set
void LALR::first() {
    bool changeFlag;
    // First, init a begin status of first set
    for (auto &p: G) {
        Symbol left = p.getLeft();
        Symbol firstRight = p.getFirstRight();
        FIRST[left].insert(firstRight);
    }
    for (Symbol i = INT; i <= ID; i=(Symbol)(i+1)) {
        FIRST[i].insert(i);
    }
    FIRST[EMPTY].insert(EMPTY);
    do {
        changeFlag = false;
        for (auto &p: G) {
            Symbol left = p.getLeft();
            size_t oldsize = FIRST[left].size();
            Symbol firstRight = p.getFirstRight();
            if (FIRST[firstRight].count(EMPTY) == 0) {
                FIRST[left].insert(FIRST[firstRight].begin(), FIRST[firstRight].end());
            } else {
                Symbol secondRight = p.getSecondRight();
                for(auto &p: FIRST[firstRight]) {
                    if (p == EMPTY) {
                        continue;
                    }
                    FIRST[left].insert(p);
                }
                FIRST[left].insert(FIRST[secondRight].begin(), FIRST[secondRight].end());
            }
            if (oldsize != FIRST[left].size()) {
                changeFlag = true;
            }
        }
    } while(changeFlag);
}

set<Symbol> LALR::first(const vector<Symbol> syms) {
    set<Symbol> ret;
    for (auto &s : syms) {
        ret.insert(FIRST[s].begin(), FIRST[s].end());
    }
    return ret;
}

//bool similar(state& s1, state&s2) {
//
//}

//void combine(state& s1, state&s2) {
//
//}

state LALR::closure(state I) {
    if (I.items.size() == 0) return I;
    // bool changeFlag = false;
    // do {
    //     size_t size = I.items.size();
    // }
    for (size_t i = 0; i < I.items.size(); ++i) { // 遍历state里的每一个形态，item
        item conf = I.items[i]; // 得到形态
        int dotPos = conf.dotPos; // 得到点的位置
        if (dotPos >= conf.prod.right.size()) {
            continue; // 点处于生成式的末尾
        }
        Symbol dotSymbol = conf.prod.right[dotPos]; // 点后面的符号
        if(dotSymbol <= ID) continue;
        vector<Symbol> searchF;
        searchF.assign(conf.lookAhead.begin(), conf.lookAhead.end());
        searchF.push_back(conf.getDotBackSymbol());
        set<Symbol> fs = first(searchF);
        for (auto &p: G) {
            if (p.getLeft() == dotSymbol) {
                item addItem = item(p, 0, fs);
                bool canCombine = false;
                for (auto &it : I.items) {
                    if (it.dotPos == 0 && it.prod== p) {
                        it.lookAhead.insert(fs.begin(), fs.end());
                        canCombine = true;
                    }
                    if (canCombine) break;
                }
                if (!canCombine) I.items.push_back(addItem);
                // lookahead 改成set<Symbol>，直接存first set
            }
        }
    }
    return I;
}

void LALR::initStates() {
    state initial;
    set<Symbol> e;
    e.insert(EMPTY);
    item initialItem(G[0], 0, e);
    initial.add(initialItem);
    C.push_back(closure(initial));
//    C.push_back(initial);
    for (size_t i = 0; i < C.size(); ++i) { // 遍历C向量，也就是遍历所有的state
//        C[i] = closure(C[i]);
        printf("CLOSURE:NO. %zu\n", i);
        // map<Symbol, int> SymbolStateNumMap;
        for (auto &p: C[i].items) { // 遍历一个state里面的所有形态，item
            if (p.dotPos >= p.prod.right.size()) continue;
            if (p.prod.right[p.dotPos] == EMPTY) continue;
            Symbol X = p.prod.right[p.dotPos];
            C[i].gotoSymbols.insert(X);
            pair<int, Symbol> gt = make_pair(i, X); // 查可以触发next（C,X）的形态
            if(GOTO.count(gt) == 0) { // 新建state=next(I,X)
                state newState;
                for (auto &pt: C[i].items) {
                    if (X == pt.prod.right[pt.dotPos]) {
                        item newStateInitItem = pt;
                        newStateInitItem.dotPos++;
                        newState.items.push_back(newStateInitItem);
                    }
                }
                newState = closure(newState);
                if (find(C.begin(), C.end(), newState) == C.end()) {
                    C.push_back(newState);
                    GOTO[gt] = C.size()-1;
                } else {
                    for (int j = 0; j < C.size(); j++) {
                        if (C[j] == newState) {
                            for (int z = 0; z < C[j].items.size(); z++) {
                                for (auto &newStateItem : newState.items) {
                                    if (C[j].items[z] == newStateItem) {
                                        C[j].items[z].lookAhead.insert(newStateItem.lookAhead.begin(), newStateItem.lookAhead.end());
                                    }
                                }
                            }
                            GOTO[gt] = j;
                        }
                    }
                }
            }
        }
    }
}


void LALR::build() {
    initStates();
    for (int i = 0; i < C.size(); ++i) {
        state & s = C[i]; // 状态
        vector<item> reducableItems;
        for (auto &it: C[i].items) { // 存一下可以reduce的items
            if (it.dotPos == it.prod.right.size()) {
                reducableItems.push_back(it);
            }
        }
//        if (C[i].items.size() == 1 && reducableItems.size() == 1) {
//            for (Symbol t = EMPTY; t <= start; t = (Symbol)(t+1)) {
//
//            }
//        }
        for (Symbol t = EMPTY; t <= start; t = (Symbol)(t+1)) {
            for (item& it : reducableItems) {
                if (t == EMPTY && it.lookAhead.count(t) != 0 && it.prod.left == start) {
                    pair<actionStat, int> p = make_pair(ACCEPT, 0);
                    ACTION[make_pair(i, t)] = p;
                    printf("Action stat: %d, action from %d ACCEPT %d with symbol %d.\n", ACCEPT, i, it.prod.num, t);
                } else if (it.lookAhead.count(t) != 0 || (C[i].items.size() == 1 && reducableItems.size() == 1) ){
                    pair<actionStat, int> p = make_pair(REDUCE, it.prod.num);
                    if (ACTION.count(make_pair(i, t)) == 0) {
                        ACTION[make_pair(i, t)] = p;
                        printf("Action stat: %d, action from %d reducing %d with symbol %d.\n", REDUCE, i, it.prod.num, t);
                    } else {
                        printf("Error: conflict. action stat: %d, action from %d reducing %d with symbol %d.\n", REDUCE, i, it.prod.num, t);
                    }
                }
            }
            pair<int, Symbol> gotoKey = make_pair(i, t); // 在GOTO中搜索所使用的键值，分别是当前state i和下一个符号t
            if (GOTO.count(gotoKey) != 0) { // 如果确实存在有从state i读取t可以去到的state的话
                // 搜索当前state中每一个符合的item，通过dot的位置判断是应该reduce还是shift
                int stateNum = GOTO[gotoKey]; // 目标state的id
                for (auto & it : s.items) { // 对于目标state里面的每个item
                    if (it.prod.right[it.dotPos] == t){
//                        if (it.lookAhead.count(t) != 0){
                        pair<actionStat, int> p = make_pair(SHIFT, stateNum);
                        if (ACTION.count(make_pair(i, t)) == 0) {
                            ACTION[make_pair(i, t)] = p;
                            printf("Action stat: %d, action from %d to %d with symbol %d.\n", SHIFT, i, stateNum, t);
                        } else {
                            if (ACTION[make_pair(i, t)] != p) {
                                if (t == ELSE) {
                                    ACTION[make_pair(i, t)] = p;
                                    printf("Action stat: %d, action from %d to %d with symbol %d.\n", SHIFT, i, stateNum, t);
                                } else {
                                    printf("Error: conflict. action stat: %d, action from %d to %d with symbol %d.\n", SHIFT, i, stateNum, t);
                                }
                                
                            }
                        }
                    }
                }
            }
        }
    }
}

//void LALR::showTable() {
//    for (int i = 0; i < C.size(); ++i) {
//        for (Symbol t = INT; t <= END; t = (Symbol)(t+1)) {
//            pair<int, Symbol> gotoKey = make_pair(i, t);
//            if (GOTO.count(gotoKey) != 0) {
//
//            }
//        }
//    }
//}

void LALR::add(const production &prod) {
    G.push_back(prod);
}

void LALR::parser() {
    status.push_back(0);
    bool success = false;
    int step = 0;
    int tkPos = 0;
    token X = tokenList[tkPos];
//    parse.push_back(token);
    while (!success) {
//    for (auto& token: tokenList) {
        printf("%s{", step == 0?"\n":"\n");
        printf("\"statusStack\": \"");
        showStatusStack();
        printf("\", \"parseStack\": \"");
        showParseStack();
        printf("\", \"Next token\": \"");
//        showStrStack();
        printf("%s", token_strs[X.tokentype]);
        printf("\", \"action\": ");
        int sTop = status.size() - 1; // 栈顶
//        parse.push_back(tokenList[tkPos]);
//        token currentToken = parse[parse.size()-1];
        token currentToken = X;
        pair<int, Symbol>p = make_pair(status[sTop], currentToken.tokentype);
        if (ACTION.find(p) == ACTION.end()) {
            break;
        }
        pair<actionStat, int> act  = ACTION[p];
        if(act.first == SHIFT) { // 移进
            printf("\"SHIFT\"}");

            status.push_back(act.second);
            parse.push_back(currentToken);
            ++tkPos;
//            X = tokenList[tkPos];
            if (tkPos < tokenList.size()) X = tokenList[tkPos];
            else {
                struct token emptyToken;
                emptyToken.tokentype = EMPTY;
                emptyToken.type = "empty";
                emptyToken.value = "";
                X = emptyToken;
            }
        } else if(act.first == REDUCE){
            production pr = G[act.second];
            printf("\"REDUCE %s->%s\"}", pr.leftStr().c_str(), pr.rightStr().c_str());

            if (pr.right.size() == 0) {}// 空串，无需出栈，直接规约
            for (unsigned i=0; i<pr.right.size(); ++i) {
                status.pop_back();
                parse.pop_back();
            }
            struct token newToken;
            newToken.tokentype = pr.left;
            newToken.type = "nonTerminal";
            newToken.value = "";
            parse.push_back(newToken);
//            ++tkPos;
//            if (tkPos < tokenList.size()) X = tokenList[tkPos];
//            else {
//                struct token emptyToken;
//                emptyToken.tokentype = EMPTY;
//                emptyToken.type = "empty";
//                emptyToken.value = "";
//                X = emptyToken;
//            }
//            X = newToken;
            status.push_back(GOTO[make_pair(status[status.size() - 1], pr.left)]);
        } else if(act.first == ACCEPT) {
            success = true;
            printf("\"ACCEPT\"}\n");
        }
        ++step;
    }
}

void LALR::run(FILE* fp) {
    initCFG();
    for (unsigned i = 0; i < G.size(); i++) {
        G[i].num = i;
    }
    build();
//    showTable();
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
    } else {
        fp = fopen("/Users/hrbattery/Documents/iroiroProject/compilerPrinciple/CParser/CParser/CParser/test.txt", "r");
    }
    printf("%s", "Result: \n");

    LALR lalr;
    lalr.run(fp);

    fclose(fp);
    fp = NULL;
    return 0;
}
