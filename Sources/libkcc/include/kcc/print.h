//
//  ast_print.h
//  kcc
//
//  Created by Christophe Bronner on 2024-07-20.
//

#include "token.h"
#include "ast_node.h"

char *token_nameof(enum token_kind kind);
char *ast_nameof(enum ast_kind kind);

void token_print(struct token token);
void print_ast(struct ast_node *node);

void ast_write(struct ast_node *node);
