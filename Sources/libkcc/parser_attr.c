//
//  parser_attr.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-08.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/attribute.h>
#include <kcc/attributes.h>
#include <kcc/diagnostics.h>
#include <kcc/lexer.h>

#include <stdlib.h>

#pragma mark - Attribute Parsing

static ast_t parse_nodiscard_attribute(parser_t parser) {
	ast_t reason = nullptr;

	if (Token.kind == T_LPAREN) {
		lexer_advance(Lexer);
		reason = parse_string_literal(parser);
		rparen();
	}

	//TODO: nodiscard_attribute node
	return nullptr;
}

static ast_t parse_maybe_unused_attribute(parser_t parser) {
	//TODO: maybe_unused_attribute node
	return nullptr;
}

static ast_t parse_deprecated_attribute(parser_t parser) {
	ast_t reason = nullptr;

	if (Token.kind == T_LPAREN) {
		lexer_advance(Lexer);
		reason = parse_string_literal(parser);
		rparen();
	}

	//TODO: deprecated_attribute node
	return nullptr;
}

static ast_t parse_fallthrough_attribute(parser_t parser) {
	//TODO: Validate using parser context
	//TODO: fallthrough_attribute node
	return nullptr;
}

static ast_t parse_noreturn_attribute(parser_t parser) {
	//TODO: noreturn_attribute node
	return nullptr;
}

static ast_t parse_unsequenced_attribute(parser_t parser) {
	//TODO: Validate using parser context
	//TODO: unsequenced_attribute node
	return nullptr;
}

static ast_t parse_reproducible_attribute(parser_t parser) {
	//TODO: Validate using parser context
	//TODO: reproducible_attribute node
	return nullptr;
}

#pragma mark - Unknown Attribute Parsing

static void balance(enum token_kind left, enum token_kind right, int *amount) {
	if (Token.kind == left)
		(*amount)++;
	else if (Token.kind != right)
		return;

	(*amount)--;

	if (*amount >= 0)
		return;

	*amount = 0;
	fatal("attribute is unbalanced");
}

//TODO: I doubt this would actually work
static ast_t parse_unknown_attribute(parser_t parser) {
	int paren = 0;
	int bracket = 0;
	int curly = 0;

	do {
		balance(T_LPAREN, T_RPAREN, &paren);
		balance(T_LBRACKET, T_RBRACKET, &bracket);
		balance(T_LCURLY, T_RCURLY, &curly);
		lexer_advance(Lexer);
	} while (bracket != -2);

	//TODO: construct unknown attribute node with that parsed text
	//TODO: unknown_attribute node
	return 0;
}

#pragma mark - Attribute Sequence Parsing

static ast_t parse_attribute_invocation(parser_t parser, enum attribute_kind attribute) {
	switch (attribute) {
	case A_NODISCARD: return parse_nodiscard_attribute(parser);
	case A_MAYBE_UNUSED: return parse_maybe_unused_attribute(parser);
	case A_DEPRECATED: return parse_deprecated_attribute(parser);
	case A_FALLTHROUGH: return parse_fallthrough_attribute(parser);
	case A_NORETURN: return parse_noreturn_attribute(parser);
	case A_UNSEQUENCED: return parse_unsequenced_attribute(parser);
	case A_REPRODUCIBLE: return parse_reproducible_attribute(parser);
	default: return nullptr;
	}
}

static bool parse_attribute_scope(parser_t parser, enum attribute_scope *scope) {
	identifier();

	if (Token.kind != T_COLON) {
		*scope = SCOPE_STANDARD;
		return true;
	}

	struct attribute_scope_syntax *scope_match = attribute_scope(TokenSource);
	if (!scope_match)
		return false;
	*scope = scope_match->scope;

	//TODO: If deprecated warn

	colon();
	colon();
	identifier();

	return true;
}

static ast_t parse_attribute(parser_t parser) {
	enum attribute_scope scope;

	if (!parse_attribute_scope(parser, &scope))
		return parse_unknown_attribute(parser);

	struct attribute_syntax *attribute_match = attribute_lookup(scope, TokenSource);
	//TODO: If deprecated warn

	parse_attribute_invocation(parser, attribute_match->attribute);
}

ast_t parse_attribute_specifier(parser_t parser) {
	if (!lexer_match(parser->lexer, T_LBRACKET))
		return nullptr;

	if (!lexer_match(parser->lexer, T_LBRACKET)) {
		//TODO: diagnostic error: missing bracket in attribute specifier
	}

	struct ast_storage attributes = {};

	bool comma = true;
	while (Token.kind != T_RBRACKET) {
		if (!comma)
			fatal("expected comma after attribute");
		comma = false;

		ast_t attribute = parse_attribute(parser);
		ast_insert(&attributes, attribute);

		if (Token.kind == T_COMMA) {
			lexer_advance(Lexer);
			comma = true;
		}
	}

	rbracket();
	rbracket();
}

ast_t parse_attribute_sequence(parser_t parser) {
	//TODO: Parse 0 or more specifiers!
	return parse_attribute_specifier(parser);
}
