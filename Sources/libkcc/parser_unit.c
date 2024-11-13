//
//  parser_unit.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/lexer1.h>

struct ast_node *parse_unit() {
	struct ast_storage declarations = {};

	while (Token.kind != T_EOF) {
		struct ast_node *declaration = parse_declaration();
		ast_insert(&declarations, declaration);
	}

	struct ast_node *node = ast_alloc();
	ast_unit(node, declarations);
	return node;
}
