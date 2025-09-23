#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

typedef struct{
    TokenType type;
    char value;
} Token;




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


typedef enum{
    NODE_VAR,
    NODE_NOT,
    NODE_IMPLIES,
    NODE_AND,
    NODE_OR,
    NODE_LPAREN,
    NODE_RPAREN,
    NODE_END
} NodeType;

typedef struct{
    NodeType type;
    char value;
    struct Node* left;
    struct Node* right;
} Node;


Node* create_Node(NodeType type, char value, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}





int main(){
    const char* proposition = "P\\/R";

    int tokenCount = 0;

    // tokanize関数の戻り値を変数に保存し、同時にトークン数も更新
    Token* tokens = tokanize(proposition, &tokenCount);
  

    // 保存したポインタとトークン数を使ってprintTokensを呼び出す
    printTokens(tokens, tokenCount);
    
    // メモリを解放する
    free(tokens);

    return 0;

}