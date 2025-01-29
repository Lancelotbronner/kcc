//
//  parser_unit.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/lexer.h>

ast_t parse_unit(parser_t parser) {
	struct ast_storage declarations = {};

	while (Token.kind != T_EOF) {
		ast_t declaration = parse_declaration(parser, true);
		ast_insert(&declarations, declaration);
	}

	ast_t node = ast_alloc();
	ast_unit(node, declarations);
	return node;
}
