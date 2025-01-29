//
//  parser.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "parser.h"

#include <kcc/lexer.h> // Token

#include <stdlib.h>

size_t parser_size() {
	return sizeof(struct parser);
}

parser_t parser_alloc() {
	return malloc(sizeof(struct parser));
}

void parser_init(parser_t parser, lexer_t lexer) {
	*parser = (struct parser){
		.lexer = lexer,
	};
}

ast_t parser_parse(parser_t parser) {
	lexer_advance(parser->lexer);
	return parse_unit(parser);
}
