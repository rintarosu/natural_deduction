#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"



int main(){
    //↓ここに字句解析、構文解析したい論理式を入力
    const char* proposition = "~~~P\\/(R->(~R/\\Q))";

    int tokenCount = 0;

    Token* tokens = tokanize(proposition, &tokenCount);
  
    printTokens(tokens, tokenCount);    

    Node* node = parse_expression(tokens);

    printTree(node,tokenCount);


    free(tokens);
    freeTree(node);

    return 0;

}