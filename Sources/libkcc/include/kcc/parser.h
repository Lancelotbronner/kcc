//
//  parser.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <kcc/types.h>

#include <stdint.h>

enum parser_mode : unsigned char {
	/// The parser is currently at the unit level.
	///
	/// Expected nodes are declarations.
	PARSER_UNIT,

	/// The parser is currently within a definition body.
	PARSER_BODY,
};

/// Returns the size of the `parser` struct.
size_t parser_size();

/// Allocates a parser.
parser_t parser_alloc();

/// Initializes a new parser reading from the specified lexer.
void parser_init(parser_t parser, lexer_t lexer);

ast_t parser_parse2(parser_t parser);

ast_t parser_parse(parser_t parser);
