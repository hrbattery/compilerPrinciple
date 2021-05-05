/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    MAIN = 259,                    /* MAIN  */
    VOID = 260,                    /* VOID  */
    BREAK = 261,                   /* BREAK  */
    DO = 262,                      /* DO  */
    ELSE = 263,                    /* ELSE  */
    IF = 264,                      /* IF  */
    WHILE = 265,                   /* WHILE  */
    RETURN = 266,                  /* RETURN  */
    READ = 267,                    /* READ  */
    WRITE = 268,                   /* WRITE  */
    LBRACE = 269,                  /* LBRACE  */
    RBRACE = 270,                  /* RBRACE  */
    LSQUARE = 271,                 /* LSQUARE  */
    RSQUARE = 272,                 /* RSQUARE  */
    LPAR = 273,                    /* LPAR  */
    RPAR = 274,                    /* RPAR  */
    SEMI = 275,                    /* SEMI  */
    PLUS = 276,                    /* PLUS  */
    MINUS = 277,                   /* MINUS  */
    MUL_OP = 278,                  /* MUL_OP  */
    DIV_OP = 279,                  /* DIV_OP  */
    AND_OP = 280,                  /* AND_OP  */
    OR_OP = 281,                   /* OR_OP  */
    NOT_OP = 282,                  /* NOT_OP  */
    ASSIGN = 283,                  /* ASSIGN  */
    LT = 284,                      /* LT  */
    GT = 285,                      /* GT  */
    SHL_OP = 286,                  /* SHL_OP  */
    SHR_OP = 287,                  /* SHR_OP  */
    EQ = 288,                      /* EQ  */
    NOTEQ = 289,                   /* NOTEQ  */
    LTEQ = 290,                    /* LTEQ  */
    GTEQ = 291,                    /* GTEQ  */
    ANDAND = 292,                  /* ANDAND  */
    OROR = 293,                    /* OROR  */
    COMMA = 294,                   /* COMMA  */
    POINT = 295,                   /* POINT  */
    INT_NUM = 296,                 /* INT_NUM  */
    IDENTIFIER = 297               /* IDENTIFIER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INT 258
#define MAIN 259
#define VOID 260
#define BREAK 261
#define DO 262
#define ELSE 263
#define IF 264
#define WHILE 265
#define RETURN 266
#define READ 267
#define WRITE 268
#define LBRACE 269
#define RBRACE 270
#define LSQUARE 271
#define RSQUARE 272
#define LPAR 273
#define RPAR 274
#define SEMI 275
#define PLUS 276
#define MINUS 277
#define MUL_OP 278
#define DIV_OP 279
#define AND_OP 280
#define OR_OP 281
#define NOT_OP 282
#define ASSIGN 283
#define LT 284
#define GT 285
#define SHL_OP 286
#define SHR_OP 287
#define EQ 288
#define NOTEQ 289
#define LTEQ 290
#define GTEQ 291
#define ANDAND 292
#define OROR 293
#define COMMA 294
#define POINT 295
#define INT_NUM 296
#define IDENTIFIER 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
