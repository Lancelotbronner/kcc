//
//  parser_parse.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-12-08.
//

#include "parser.h"

#include <kcc/diagnostics.h>
#include <kcc/lexer.h>

//MARK: - Parsing Utilities

void check_specifier(bool is_specifier) {
	if (!is_specifier)
		return;
	//TODO: diagnostic warning: duplicate specifier has no effect [fix-it: remove]
}

//MARK: - Parsing Modes

/// Handles parsing common to every modes.
static bool parser_universal(parser_t parser) {
	switch (Token.kind) {
		// ignore the following tokens
	case T_UNKNOWN:
	case T_INDENTATION:
	case T_WHITESPACE:
	case T_NEWLINE:
	case T_COMMENT:
		return true;

	case T_CONST:
		check_specifier(parser->is_const);
		parser->is_const = true;
		return true;

	case T_RESTRICT:
		check_specifier(parser->is_restrict);
		parser->is_restrict = true;
		return true;

	case T_VOLATILE:
		check_specifier(parser->is_volatile);
		parser->is_volatile = true;
		return true;

	case T_ATOMIC:
		check_specifier(parser->is_atomic);
		parser->is_atomic = true;
		return true;

	case T_AUTO:
		check_specifier(parser->is_auto);
		parser->is_auto = true;
		return true;

	case kconstexpr:
		check_specifier(parser->is_constexpr);
		parser->is_constexpr = true;
		return true;

	case kextern:
		check_specifier(parser->is_extern);
		parser->is_extern = true;
		return true;

	case kregister:
		check_specifier(parser->is_register);
		parser->is_register = true;
		return true;

	case kstatic:
		check_specifier(parser->is_static);
		parser->is_static = true;
		return true;

	case kthread_local:
		check_specifier(parser->is_thread_local);
		parser->is_thread_local = true;
		return true;

	case ktypedef:
		check_specifier(parser->is_typedef);
		parser->is_typedef = true;
		return true;

	default:
		//TODO: warn invalid token
		return true;
	}
}

/// Handles parsing within a unit.
static bool parser_unit(parser_t parser) {
	switch (Token.kind) {
	case T_DOCUMENTATION:
		//TODO: insert this in the documentation store
		// it will be associated to the next declaration.
		// other types of nodes may clear it
		return true;

	case T_INLINE:
		check_specifier(parser->is_inline);
		parser->is_inline = true;
		return true;

	case T_NORETURN:
		check_specifier(parser->is_noreturn);
		parser->is_noreturn = true;
		//TODO: diagnostic: _NoReturn specifier is deprecated [fix-it: convert to attribute]
		return true;

	default:
		return false;
	}
}

/// Handles parsing within a body.
static bool parser_body(parser_t parser) {
	switch (Token.kind) {
		// ignore the following tokens
	case T_DOCUMENTATION:
		return true;

		// diagnose function specifier in body
	case T_INLINE:
	case T_NORETURN:
		return true;

	default:
		return false;
	}
}

/// Handles mode-specific parsing.
static bool parse_mode(parser_t parser) {
	switch (parser->mode) {
	case PARSER_UNIT: return parser_unit(parser);
	case PARSER_BODY: return parser_body(parser);
	}
}

static bool parser_consume(parser_t parser) {
	if (Token.kind == T_EOF)
		return false;

	// return early if the token was handled.
	if (parser_universal(parser))
		return true;
	if (parse_mode(parser))
		return true;

	// attempt recovery from invalid tokens
	fatalt("invalid token while parsing");
}

ast_t parser_parse2(parser_t parser) {
	do lexer_advance(parser->lexer);
	while (parser_consume(parser));

	// TODO: diagnose anything started but not completed?
}
