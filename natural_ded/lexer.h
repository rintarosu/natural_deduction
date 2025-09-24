#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
    TOK_VAR,
    TOK_NOT,
    TOK_IMPLIES,
    TOK_AND,
    TOK_OR,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_END
} TokenType;

// 字句を表す構造体
typedef struct {
    TokenType type;
    char value;
} Token;


Token* tokanize(const char* input, int* tokenCount);


void printTokens(Token* tokens, int count);

#endif