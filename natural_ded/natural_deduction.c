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

static Token* tokens;
static int pos;


void consume(TokenType expectedType) {
    if (tokens[pos].type == expectedType) { pos++; } 
    else { fprintf(stderr, "Error: Unexpected token. Expected %d, got %d\n", expectedType, tokens[pos].type); exit(1); }
}

Node* parse_implies();
Node* parse_not();
Node* parse_and();
Node* parse_or();
Node* parse_factor();



Node* parse_factor() {
    Token current = tokens[pos];
    if (current.type == TOK_VAR) {
        consume(TOK_VAR);
        return create_Node(NODE_VAR, current.value, NULL, NULL);
    } else if (current.type == TOK_LPAREN) {
        consume(TOK_LPAREN);
        Node* node = parse_implies(); // 変更: parse() を parse_implies() に
        consume(TOK_RPAREN);
        return node;
    } else {
        fprintf(stderr, "Error: Unexpected token in factor\n");
        exit(1);
    }
}

Node* parse_not() {
    if (tokens[pos].type == TOK_NOT) {
        consume(TOK_NOT);
        Node* operand = parse_not();
        return create_Node(NODE_NOT, 0, operand, NULL);
    } else {
        return parse_factor();
    }
}

Node* parse_and() {
    Node* left = parse_not();
    while (tokens[pos].type == TOK_AND) {
        consume(TOK_AND);
        Node* right = parse_not();
        left = create_Node(NODE_AND, 0, left, right);
    }
    return left;
}

Node* parse_or() {
    Node* left = parse_and();
    while (tokens[pos].type == TOK_OR) {
        consume(TOK_OR);
        Node* right = parse_and();
        left = create_Node(NODE_OR, 0, left, right);
    }
    return left;
}

Node* parse_implies() { 
    Node* left = parse_or();
    if (tokens[pos].type == TOK_IMPLIES) {
        consume(TOK_IMPLIES);
        Node* right = parse_implies();
        return create_Node(NODE_IMPLIES, 0, left, right);
    }
    return left;
}


Node* parse_expression(Token* token_list) {
    tokens = token_list;
    pos = 0;
    Node* root = parse_implies(); // 変更: parse() を parse_implies() に
    if (tokens[pos].type != TOK_END) {
        fprintf(stderr, "Error: Syntax error at the end of input\n");
        exit(1);
    }
    return root;
}


void printTree(Node* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) { printf("  "); }
    switch (node->type) {
        case NODE_VAR: printf("VAR(%c)\n", node->value); break;
        case NODE_NOT: printf("NOT\n"); break;
        case NODE_IMPLIES: printf("IMPLIES\n"); break;
        case NODE_AND: printf("AND\n");break;
        case NODE_OR: printf("OR\n");break;
    }
    printTree(node->left, level + 1);
    printTree(node->right, level + 1);
}

void freeTree(Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}


int main(){
    const char* proposition = "P\\/R->E/\\Q";

    int tokenCount = 0;

    // tokanize関数の戻り値を変数に保存し、同時にトークン数も更新
    Token* tokens = tokanize(proposition, &tokenCount);
  

    // 保存したポインタとトークン数を使ってprintTokensを呼び出す
    printTokens(tokens, tokenCount);
    


    // メモリを解放する
    

    Node* node = parse_expression(tokens);

    printTree(node,tokenCount);


    free(tokens);
    freeTree(node);

    return 0;

}