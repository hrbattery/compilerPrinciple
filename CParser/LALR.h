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
    private:
        Symbol left;
        vector<Symbol> right;
        friend bool operator == (const production &prodA, const production &prodB) {
            return prodA.left == prodB.left && prodA.right == prodB.right;
        }
    public:
        production(const Symbol left, const vector<Symbol> right):
            left(left), right(right){}
        production(const production &prod) {
            left = prod.left;
            right = prod.right;
        }
        Symbol getLeft();
        Symbol getFirstRight();
};

// 形态/项目，包含产生式，position和lookahead的信息
class item {
    friend class state;
    private:
        production prod;
        int dotPos;
        Symbol lookAhead;
        friend bool operator == (const item &itemA, const item &itemB) {
            return itemA.prod == itemB.prod && itemA.dotPos == itemB.dotPos && itemA.lookAhead == itemB.lookAhead;
        }
    public:
        item(const production prod, const int dotPos, const Symbol lookAhead):
            prod(prod), dotPos(dotPos), lookAhead(lookAhead) {}
};

// 状态，形态的闭包
class state {
    friend class item;
    private:
        vector<item> items;
        int stateNum;
        friend bool operator == (const state &stateA, const state &stateB) {
            if (stateA.items.size() != stateB.items.size()) return false;
            for (const auto &p: stateA.items) {
                auto it = find(stateB.items.begin(), stateB.items.end(), p);
                if (it == stateB.items.end()) return false;
                if (it->dotPos != p.dotPos || it->lookAhead != p.lookAhead) return false;
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
            GOTO,
            REDUCE
        };

        vector<state> C; // 状态集
        map<Symbol,set<Symbol>> FIRST; // 每个Symbol的First集
        map<Symbol,set<Symbol>> FOLLOW; // 每个Symbol的Follow集
        vector<token> tokenList;
        vector<int> status;
        vector<char> parse;
        map<pair<int, Symbol>,int> GOTO;
        map<pair<int, Symbol>, pair<actionStat, int>> ACTION;

        void first();
        void first(const Symbol sym);
        void first(const vector<Symbol> syms);
        void follow();
        state closure(state I);
        state Goto(const Item &I, Symbol X);
        void items(); // 求项目集状态机DFA！!
		void showStrStack(); // 显示输入栈
		void showStatusStack();
		void showParseStack();
    public:
        void initCFG();
        void add(const production &prod); // 添加产生式
		void build(); // 构造Action、GOTO表
		void showTable(); // 打印LR分析表！
		void debug();
		void loadStr(const string &in); // 读取输入串
		void parser(); // LR(1)分析
		void showGrammar(); // 显示输入的文法产生式
		void drawGraph(); // 画出DFA！
		void generateDot();
		void run(FILE* fp);
};