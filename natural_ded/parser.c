#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"



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
        Node* node = parse_implies(); 
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
    Node* root = parse_implies(); 
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