//
//  parser_literal.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

#include "parser.h"
#include "globals.h"

#include "lexer1.h"

#include <string.h>

//MARK: - String Literal

static enum token_encoding_modifier parse_encoding_prefix() {
	if (strcmp(Text, "u8") == 0)
		return ENCODING_UTF8;
	if (strcmp(Text, "u") == 0)
		return ENCODING_UTF16;
	if (strcmp(Text, "U") == 0)
		return ENCODING_UTF32;
	if (strcmp(Text, "L") == 0)
		return ENCODING_WIDE;
}

struct ast_node *parse_string_literal() {
	enum token_encoding_modifier encoding = ENCODING_NONE;

	if (Token.kind == T_IDENTIFIER) {
		encoding = parse_encoding_prefix();
		scan();
	}

	//TODO: Complete string literals
	return nullptr;
}
