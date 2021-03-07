%{
#include "token.h"
int cur_line_num = 1;
void init_scanner();
void lex_error(char* msg, int line);
%}

INTLITERAL          ([0-9]+)
ID                  ([a-zA-Z][a-zA-Z0-9]*)

%%
[\n]                { cur_line_num++;   }
[ \t\r\a]+          { /* ignore */      }
--.*\n              { /* comments */    }

"begin"             { return BEGINSYM;  }
"end"               { return END;       }
"read"              { return READ;      }
"write"             { return WRITE;     }
"("                 { return LPAREN;    }
")"                 { return RPAREN;    }
";"                 { return SEMICOLON; }
","                 { return COMMA;     }
":="                { return ASSIGNOP;  }
"+"                 { return PLUOP;     }
"-"                 { return MINUSOP;   }
"SCANEOF"           { return SCANEOF;   }

{INTLITERAL}        { return INTLITERAL;}
{ID}                { return ID;        }
<<EOF>>             { return 0; }

.                   { lex_error("Unrecognized character", cur_line_num); }

%%

int main(int argc, char* argv[]) {
    int token;
    init_scanner();
    while (token = yylex()) {
        print_token(token);
        puts(yytext);
    }
    return 0;
}

void init_scanner() {
    printf("%-20s%s\n", "TOKEN-TYPE", "TOKEN-VALUE");
    printf("--------------------------------------\n");
}

void lex_error(char* msg, int line) {
    printf("\n Error at line %d: %s\n\n", line, msg);
}

int yywrap() {
    return 1;
}