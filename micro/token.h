#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>

typedef enum TOKEN_TYPE{
    BEGINSYM = 256, END, READ, WRITE, ID, INTLITERAL,
    LPAREN, RPAREN, SEMICOLON, COMMA, 
    ASSIGNOP,PLUOP, MINUSOP, SCANEOF
} token;

extern token scanner(void);

static void print_token(int token) {
    static char* token_strs[] = {
        "BEGIN", "END", "READ", "WRITE", "ID", "INTLITERAL",
        "LPAREN", "RPAREN", "SEMICOLON", "COMMA", 
        "ASSIGNOP", "PLUOP", "MINUSOP", "SCANEOF"
    };
    if (token < 256) {
        printf("%-20c", token);
    } else {
        printf("%-20s", token_strs[token-256]);
    }
}

#endif