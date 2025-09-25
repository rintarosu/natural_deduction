#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "natural_deduction.h"

extern Node** assumptions;
extern int assumption_count;


int main(){
    //↓ここに字句解析、構文解析したい論理式を入力
    const char* proposition = "~~~P\\/(R->(~R/\\Q))";

    int tokenCount = 0;

    Token* tokens = tokanize(proposition, &tokenCount);
  
    printTokens(tokens, tokenCount);    

    Node* node = parse_expression(tokens);

    printTree(node,tokenCount);

   
    assumptions = (Node**)realloc(assumptions, (assumption_count + 1) * sizeof(Node*));
    if (assumptions == NULL) {
        fprintf(stderr, "メモリの初期割り当てに失敗しました\n");
        return 1;
    }



    free(tokens);
    freeTree(node);

    return 0;

}