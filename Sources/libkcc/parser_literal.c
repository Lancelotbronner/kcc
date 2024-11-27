//
//  parser_literal.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

#include "parser.h"
#include "globals.h"

#include "lexer.h"

#include <string.h>

//MARK: - String Literal

static enum token_encoding_modifier parse_encoding_prefix() {
	if (strcmp(TokenSource, "u8") == 0)
		return ENCODING_UTF8;
	if (strcmp(TokenSource, "u") == 0)
		return ENCODING_UTF16;
	if (strcmp(TokenSource, "U") == 0)
		return ENCODING_UTF32;
	if (strcmp(TokenSource, "L") == 0)
		return ENCODING_WIDE;
}

struct ast_node *parse_string_literal() {
	enum token_encoding_modifier encoding = ENCODING_NONE;

	if (Token.kind == T_IDENTIFIER) {
		encoding = parse_encoding_prefix();
		lexer_advance();
	}

	//TODO: Complete string literals
	return nullptr;
}
