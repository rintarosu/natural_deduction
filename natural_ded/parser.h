#ifndef PARSER.H
#define LEXER_H

#include "lexer.h"

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

Node* create_Node(NodeType type, char value, Node* left, Node* right);

static Token* tokens;
static int pos;

void consume(TokenType expectedType);

Node* parse_implies();
Node* parse_not();
Node* parse_and();
Node* parse_or();
Node* parse_factor();
Node* parse_expression(Token* token_list);
void printTree(Node* node, int level);
void freeTree(Node* node);

#endif