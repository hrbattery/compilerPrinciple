#ifndef token_h
#define token_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef enum tokenType {
    INT, MAIN, VOID, BREAK, DO,
    ELSE, IF, WHILE, RETURN, READ,
    WRITE,
    LBRACE, RBRACE, LSQUARE, RSQUARE, LPAR,
    RPAR, SEMI, PLUS, MINUS, MUL_OP,
    DIV_OP, AND_OP, OR_OP, NOT_OP, ASSIGN,
    LT, GT, SHL_OP, SHR_OP, EQ,
    NOTEQ, LTEQ, GTEQ, ANDAND, OROR,
    COMMA, POINT,
    INT_NUM, ID
} tokenType;

typedef struct token {
    string      value;
    string      type;
    tokenType   tokentype;
} token;

enum class State {
    Begin,
    InIdentifier,
    InInteger,
};

static const char* token_strs[] = {
    "INT", "MAIN", "VOID", "BREAK", "DO",
    "ELSE", "IF", "WHILE", "RETURN", "READ",
    "WRITE",
    "LBRACE", "RBRACE", "LSQUARE", "RSQUARE", "LPAR",
    "RPAR", "SEMI", "PLUS", "MINUS", "MUL_OP",
    "DIV_OP", "AND_OP", "OR_OP", "NOT_OP", "ASSIGN",
    "LT", "GT", "SHL_OP", "SHR_OP", "EQ",
    "NOTEQ", "LTEQ", "GTEQ", "ANDAND", "OROR",
    "COMMA", "POINT",
    "INT_NUM", "ID"
};

vector<token> tokenList;

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

void addToken(string type, tokenType tokentype, string actualValue) {
    token newToken;
    newToken.value = actualValue;
    if (reserve.count(actualValue) != 0) {
        newToken.tokentype = reserve[actualValue];
        newToken.type = "Reserve";
    } else {
        newToken.tokentype = tokentype;
        newToken.type = type;
    }
    if (newToken.tokentype == INT_NUM && (tokenList[tokenList.size()-1].tokentype == INT_NUM || tokenList[tokenList.size()-1].tokentype == ID)) {
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
    printf("TOKEN: %-20s%s\n", token_strs[newToken.tokentype], newToken.value.c_str());
}

void raiseError(void) {
    printf("%s", "Error: Lexical Error.\n");
    exit(0);
}

FILE* scan(FILE* fp) {
    char ch, nextch;
    string buf = "";
    State state = State::Begin;
    while (1) {
        ch = fgetc(fp);
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
                case '+':
                    nextch = fgetc(fp);
                    if ('0' <= nextch && nextch <= '9') {
                        state = State::InInteger;
                        buf += ch;
                        buf += nextch;
                    } else {
                        addToken("Symbol", PLUS, "+");
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
                    }
                    break;
                case '*':
                    addToken("Symbol", MUL_OP, "*");
                    break;
                case '/':
                    addToken("Symbol", DIV_OP, "/");
                    break;
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
                    if ('0' <= ch && ch <= '9') {
                        state = State::InInteger;
                        buf += ch;
                    } else if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')) {
                        state = State::InIdentifier;
                        buf += ch;
                    } else {
                        raiseError();
                    }
                break;
            }
            break;
        case State::InInteger:
            if ('0' <= ch && ch <= '9') {
                buf += ch;
            } else {
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
                addToken("Identifier", ID, buf);
                buf = "";
                state = State::Begin;
                fseek(fp, -1, SEEK_CUR);
            }
            break;
        }
    }
    return fp;
}
// FILE* scan(FILE* fp) {
//     char ch;
//     string buf = "";
//     ch = getFirstNonblank(fp);
//     if (feof(fp)) return fp;
//     if (isLetter(ch)) {
//         while ()
//     }
// }

#endif