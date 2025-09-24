#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"



Token* tokanize(const char* input, int* tokenCount) {
    int len = strlen(input);
    Token* tokens = malloc(sizeof(Token) * (len + 1));
    if (tokens == NULL) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    int pos = 0;
    int tpos = 0;
    while (pos < len) {
        char c = input[pos];
        if (isspace(c)) { pos++; continue; }
        else if (isupper(c)) { tokens[tpos++] = (Token){TOK_VAR, c}; pos++; }
        else if (c == '~') { tokens[tpos++] = (Token){TOK_NOT, 0}; pos++; }
        else if (c == '-') {
            if (pos +1 < len && input[pos+1] == '>') { tokens[tpos++] = (Token){TOK_IMPLIES, 0}; pos += 2; }
            else { fprintf(stderr, "Error: '-' の後に '>' が必要です\n"); exit(1); }}
        else if (c == '/') {
            if (pos +1 < len && input[pos+1] == '\\') { tokens[tpos++] = (Token){TOK_AND, 0}; pos += 2; }
            else { fprintf(stderr, "Error: '/' の後に '\\' が必要です\n"); exit(1); }}
        else if (c == '\\') {
            if (pos +1 < len && input[pos+1] == '/') { tokens[tpos++] = (Token){TOK_OR, 0}; pos += 2; }
            else { fprintf(stderr, "Error: '\\' の後に '/' が必要です\n"); exit(1); }}         
        else if (c == '(') { tokens[tpos++] = (Token){TOK_LPAREN, 0}; pos++; }
        else if (c == ')') { tokens[tpos++] = (Token){TOK_RPAREN, 0}; pos++; }
        else { fprintf(stderr, "Error: 不明な文字 '%c'\n", c); exit(1); }
    }
    tokens[tpos++] = (Token){TOK_END, 0};
    *tokenCount = tpos;
    return tokens;
}

void printTokens(Token* tokens, int count) {
    printf("--- Tokens ---\n");
    for (int i = 0; i < count; i++) {
        switch (tokens[i].type) {
            case TOK_VAR: printf("VAR(%c) ", tokens[i].value); break;
            case TOK_NOT: printf("NOT "); break;
            case TOK_AND: printf("AND "); break;
            case TOK_OR: printf("OR "); break;
            case TOK_IMPLIES: printf("IMPLIES "); break;
            case TOK_LPAREN: printf("LPAREN "); break;
            case TOK_RPAREN: printf("RPAREN "); break;
            case TOK_END: printf("END "); break;
        }
    }
    printf("\n\n");
}