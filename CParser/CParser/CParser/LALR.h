//
//  LALR.h
//  CParser
//
//  Created by HRBattery on 2021/4/14.
//

#ifndef LALR_h
#define LALR_h

#include <iostream>
#include <ctype.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include "token.h"
using namespace std;
#define EXTENSION_NOTERMINAL '^'

// 产生式
class production {
    friend class item;
    friend class state;
    friend class LALR;
    private:
        Symbol left;
        vector<Symbol> right;
        int num;
        friend bool operator == (const production &prodA, const production &prodB) {
            return prodA.left == prodB.left && prodA.right == prodB.right;
        }
    public:
        production(const Symbol left, const vector<Symbol> right):
            left(left), right(right){}
        production(const production &prod) {
            left = prod.left;
            right = prod.right;
            num = prod.num;
        }
        Symbol getLeft();
        Symbol getFirstRight();
        Symbol getSecondRight();
        string leftStr();
        string rightStr();
};

// 形态/项目，包含产生式，position和lookahead的信息
class item {
    friend class state;
    friend class LALR;
    private:
        production prod;
        int dotPos;
        set<Symbol> lookAhead;
        friend bool operator == (const item &itemA, const item &itemB) {
            return itemA.prod == itemB.prod && itemA.dotPos == itemB.dotPos;
//            && itemA.lookAhead == itemB.lookAhead;
        }
    public:
        item(const production prod, const int dotPos, const set<Symbol> lookAhead):
            prod(prod), dotPos(dotPos), lookAhead(lookAhead) {}
        Symbol getDotBackSymbol();
};

// 状态，形态的闭包
class state {
    friend class item;
    friend class LALR;
    private:
        vector<item> items;
        set<Symbol> gotoSymbols;
        // int stateNum;
        friend bool operator == (const state &stateA, const state &stateB) {
            if (stateA.items.size() != stateB.items.size()) return false;
            for (const auto &p: stateA.items) {
                auto it = find(stateB.items.begin(), stateB.items.end(), p);
                if (it == stateB.items.end()) return false;
//                if (it->dotPos != p.dotPos || it->lookAhead != p.lookAhead) return false;
            }
            return true;
        }
    public:
        void add(const item &conf);
};

class LALR {
    private:
        vector<production> G; // 产生式集
        enum actionStat {
            ACCEPT,
            SHIFT,
            REDUCE
        };

        vector<state> C; // 状态集
        map<Symbol,set<Symbol>> FIRST; // 每个Symbol的First集
        vector<token> tokenList;
        vector<int> status;
        vector<token> parse;
        map<pair<int, Symbol>,int> GOTO;
        map<pair<int, Symbol>, pair<actionStat, int>> ACTION;

        void first();
        void first(const Symbol sym);
        set<Symbol> first(const vector<Symbol> syms);
        state closure(state I);
        state Goto(const state &I, Symbol X);
        void initStates(); // 求项目集状态机DFA！!
        void showStatusStack();
        void showParseStack();
    public:
        void initCFG();
        void add(const production &prod); // 添加产生式
        void build(); // 构造Action、GOTO表
        void showTable(); // 打印LR分析表！
        void parser(); // LR(1)分析
        void run(FILE* fp);
};

#endif /* LALR_h */
