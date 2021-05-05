#ifndef token_h
#define token_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// tokenType: An enumerate type fot the type of token.
typedef enum tokenType {
    EMPTY, INT, MAIN, VOID, BREAK,
    DO, ELSE, IF, WHILE, RETURN,
    READ, WRITE, LBRACE, RBRACE, LSQUARE,
    RSQUARE, LPAR, RPAR, SEMI, PLUS,
    MINUS, MUL_OP, DIV_OP, AND_OP, OR_OP,
    NOT_OP, ASSIGN, LT, GT, SHL_OP,
    SHR_OP, EQ, NOTEQ, LTEQ, GTEQ,
    ANDAND, OROR, COMMA, POINT, INT_NUM,
    ID,
    program, var_decls, var_decl, decl_list, decl,
    code_block, stmts, stmt, ctrl_stmt, rw_stmt,
    assign_stmt, if_statement, if_stmt, while_stmt, do_while_stmt,
    ret_stmt, r_stmt, w_stmt, expr, expr2,
    expr3, expr4, expr5, expr6, expr7,
    expr8, expr9, expr10, expr11, start
//    ,END
} tokenType;

typedef tokenType Symbol;

// token: Save the information of token element.
typedef struct token {
    string      value;
    string      type;
    tokenType   tokentype;
} token;

// State: Indicate the state of DFA.
enum class State {
    Begin,
    InIdentifier,
    InInteger,
};

// token_strs: An one-to-one mapping from tokenType to const char[].
static const char* token_strs[] = {
    "EMPTY", "INT", "MAIN", "VOID", "BREAK", "DO",
    "ELSE", "IF", "WHILE", "RETURN", "READ",
    "WRITE",
    "LBRACE", "RBRACE", "LSQUARE", "RSQUARE", "LPAR",
    "RPAR", "SEMI", "PLUS", "MINUS", "MUL_OP",
    "DIV_OP", "AND_OP", "OR_OP", "NOT_OP", "ASSIGN",
    "LT", "GT", "SHL_OP", "SHR_OP", "EQ",
    "NOTEQ", "LTEQ", "GTEQ", "ANDAND", "OROR",
    "COMMA", "POINT",
    "INT_NUM", "ID",
    "program", "var_decls", "var_decl", "decl_list", "decl",
    "code_block", "stmts", "stmt", "ctrl_stmt", "rw_stmt",
    "assign_stmt", "if_statement", "if_stmt", "while_stmt", "do_while_stmt", "ret_stmt",
    "r_stmt", "w_stmt", "expr", "expr2", "expr3", "expr4", "expr5", "expr6",
    "expr7", "expr8", "expr9", "expr10", "expr11",
    "start",  "END"
};

// tokenList: Save the tokens.
vector<token> tokenList;

// reverse: A lookup table for looking up reserve words.
unordered_map<string, tokenType> reserve = {
    {"int", INT},
    {"main", MAIN},
    {"void", VOID},
    {"break", BREAK},
    {"do", DO},
    {"else", ELSE},
    {"if", IF},
    {"while", WHILE},
    {"return", RETURN},
    {"scanf", READ},
    {"printf", WRITE}
};

// addToken(): Function that add token to tokenList and print the scanning result.
void addToken(string type, tokenType tokentype, string actualValue) {
    token newToken;
    newToken.value = actualValue;
    if (reserve.count(actualValue) != 0) {
        // Look up the reserve table to check the token is reserve word or not
        newToken.tokentype = reserve[actualValue];
        newToken.type = "Reserve";
    } else {
        newToken.tokentype = tokentype;
        newToken.type = type;
    }
    if (newToken.tokentype == INT_NUM && (tokenList[tokenList.size()-1].tokentype == INT_NUM || tokenList[tokenList.size()-1].tokentype == ID)) {
        /* This is to solve the problem of symbol '+' and '-'.
         * The problem will only possibly occur when scanning an 'INT_NUM'
         * token and the previous token is an 'INT_NUM' token or an 'ID' token.
         * If happens, split the scanned 'INT_NUM' token into a 'PLUS'/'MINUS'
         * token and a 'INT_NUM' token.
         */
        token plusNewToken;
        plusNewToken.type = "Symbol";
        if (actualValue[0] == '+') {
            plusNewToken.value = '+';
            plusNewToken.tokentype = PLUS;
        } else {
            plusNewToken.value = '-';
            plusNewToken.tokentype = MINUS;
        }
        tokenList.push_back(plusNewToken);
        printf("TOKEN: %-20s%s\n", token_strs[plusNewToken.tokentype], plusNewToken.value.c_str());
        newToken.value = actualValue.substr(1);
    }
    tokenList.push_back(newToken);
    // Print the token information to stdout
    printf("TOKEN: %-20s%s\n", token_strs[newToken.tokentype], newToken.value.c_str());
}

// raiseError(): Function that raise error.
void raiseError(void) {
    printf("%s", "Error: Lexical Error.\n");
    exit(0);
}

// scan(): The implementation of the DFA.
vector<token> scan(FILE* fp) {
    /* ch: Current scanned character.
     * nextch: Next scanned character. Used when needed.
     * buf: Save the raw literal of token.
     * state: Save the current state of the DFA.
     */
    char ch, nextch;
    string buf = "";
    State state = State::Begin;
    while (1) {
        ch = fgetc(fp);
        // If reach the EOF, immediately output the token.
        if (feof(fp)) {
            switch (state)
            {
            case State::InInteger:
                addToken("Integer", INT_NUM, buf);
                break;
            case State::InIdentifier:
                addToken("Identifier", ID, buf);
                break;
            default:
                break;
            }
            break;
        }
        switch (state)
        {
        case State::Begin:
            switch (ch) {
                // Most of the cases are for symbols.
                case '{':
                    addToken("Symbol", LBRACE, "{");
                    break;
                case '}':
                    addToken("Symbol", RBRACE, "}");
                    break;
                case '[':
                    addToken("Symbol", LSQUARE, "[");
                    break;
                case ']':
                    addToken("Symbol", RSQUARE, "]");
                    break;
                case '(':
                    addToken("Symbol", LPAR, "(");
                    break;
                case ')':
                    addToken("Symbol", RPAR, ")");
                    break;
                case ';':
                    addToken("Symbol", SEMI, ";");
                    break;
                // For '+' and '-', need to indicate it is for a unasy operator or a binary operater.
                case '+':
                    nextch = fgetc(fp);
                    if ('0' <= nextch && nextch <= '9') {
                        state = State::InInteger;
                        buf += ch;
                        buf += nextch;
                    } else {
                        addToken("Symbol", PLUS, "+");
                        fseek(fp, -1, SEEK_CUR);
                    }
                    break;
                case '-':
                    nextch = fgetc(fp);
                    if ('0' <= nextch && nextch <= '9') {
                        state = State::InInteger;
                        buf += ch;
                        buf += nextch;
                    } else {
                        addToken("Symbol", MINUS, "-");
                        fseek(fp, -1, SEEK_CUR);
                    }
                    break;
                case '*':
                    addToken("Symbol", MUL_OP, "*");
                    break;
                case '/':
                    addToken("Symbol", DIV_OP, "/");
                    break;
                // For some symbols we need to look forward a character to indicate the symbol.
                // Using fseek() to correctly set the file pointer.
                case '&':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '&':
                            addToken("Symbol", ANDAND, "&&");
                            break;
                        default:
                            addToken("Symbol", AND_OP, "&");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case '|':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '|':
                            addToken("Symbol", OROR, "||");
                            break;
                        default:
                            addToken("Symbol", OR_OP, "|");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case '!':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '=':
                            addToken("Symbol", NOTEQ, "!=");
                            break;
                        default:
                            addToken("Symbol", NOT_OP, "!");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case '=':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '=':
                            addToken("Symbol", EQ, "==");
                            break;
                        default:
                            addToken("Symbol", ASSIGN, "=");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case '<':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '<':
                            addToken("Symbol", SHL_OP, "<<");
                            break;
                        case '=':
                            addToken("Symbol", LTEQ, "<=");
                            break;
                        default:
                            addToken("Symbol", LT, "<");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case '>':
                    nextch = fgetc(fp);
                    switch (nextch) {
                        case '>':
                            addToken("Symbol", SHR_OP, ">>");
                            break;
                        case '=':
                            addToken("Symbol", GTEQ, "<=");
                            break;
                        default:
                            addToken("Symbol", GT, ">");
                            fseek(fp, -1, SEEK_CUR);
                        break;
                    }
                    break;
                case ',':
                    addToken("Symbol", COMMA, ",");
                    break;
                case '.':
                    addToken("Symbol", POINT, ".");
                    break;
                case ' ': case '\t': case '\r': case '\n':
                    break;
                default:
                    // Goto InInteger state
                    if ('0' <= ch && ch <= '9') {
                        state = State::InInteger;
                        buf += ch;
                    // Goto InIdentifier state
                    } else if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')) {
                        state = State::InIdentifier;
                        buf += ch;
                    } else {
                    // If we cannot find a path for current input, the input character must be invalid.
                        raiseError();
                    }
                break;
            }
            break;
        case State::InInteger:
            if ('0' <= ch && ch <= '9') {
                buf += ch;
            } else {
                // Meet the end of the integer.
                addToken("Integer", INT_NUM, buf);
                buf = "";
                state = State::Begin;
                fseek(fp, -1, SEEK_CUR);
            }
            break;
        case State::InIdentifier:
            if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9') || (ch == '_')) {
                buf += ch;
            } else {
                // Meet the end of the identifier.
                addToken("Identifier", ID, buf);
                buf = "";
                state = State::Begin;
                fseek(fp, -1, SEEK_CUR);
            }
            break;
        }
    }
//    struct token newToken;
//    newToken.tokentype = END;
//    newToken.type = "end";
//    newToken.value = "";
//    tokenList.push_back(newToken);
    return tokenList;
}

#endif
