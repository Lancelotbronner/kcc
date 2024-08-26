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
#include <kcc/scanner.h>

#include <stdlib.h>

#pragma mark - Attribute Parsing

static struct ast_node *parse_nodiscard_attribute() {
	struct ast_node *reason = nullptr;

	if (Token.kind == T_LPAREN) {
		scan();
		reason = parse_string_literal();
		rparen();
	}

	//TODO: nodiscard_attribute node
	return nullptr;
}

static struct ast_node *parse_maybe_unused_attribute() {
	//TODO: maybe_unused_attribute node
	return nullptr;
}

static struct ast_node *parse_deprecated_attribute() {
	struct ast_node *reason = nullptr;

	if (Token.kind == T_LPAREN) {
		scan();
		reason = parse_string_literal();
		rparen();
	}

	//TODO: deprecated_attribute node
	return nullptr;
}

static struct ast_node *parse_fallthrough_attribute() {
	//TODO: Validate using parser context
	//TODO: fallthrough_attribute node
	return nullptr;
}

static struct ast_node *parse_noreturn_attribute() {
	//TODO: noreturn_attribute node
	return nullptr;
}

static struct ast_node *parse_unsequenced_attribute() {
	//TODO: Validate using parser context
	//TODO: unsequenced_attribute node
	return nullptr;
}

static struct ast_node *parse_reproducible_attribute() {
	//TODO: Validate using parser context
	//TODO: unsequenced_attribute node
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
static struct ast_node *parse_unknown_attribute() {
	int paren = 0;
	int bracket = 0;
	int curly = 0;

	do {
		balance(T_LPAREN, T_RPAREN, &paren);
		balance(T_LBRACKET, T_RBRACKET, &bracket);
		balance(T_LCURLY, T_RCURLY, &curly);
		scan();
	} while (bracket != -2);

	//TODO: construct unknown attribute node with that parsed text
	return 0;
}

#pragma mark - Attribute Sequence Parsing

static struct ast_node *parse_attribute_invocation(enum attribute_kind attribute) {
	switch (attribute) {
	case A_NODISCARD: return parse_nodiscard_attribute();
	case A_MAYBE_UNUSED: return parse_maybe_unused_attribute();
	case A_DEPRECATED: return parse_deprecated_attribute();
	case A_FALLTHROUGH: return parse_fallthrough_attribute();
	case A_NORETURN: return parse_noreturn_attribute();
	case A_UNSEQUENCED: return parse_unsequenced_attribute();
	case A_REPRODUCIBLE: return parse_reproducible_attribute();
	default: return nullptr;
	}
}

static bool parse_attribute_scope(enum attribute_scope *scope) {
	identifier();

	if (Token.kind != T_COLON) {
		*scope = SCOPE_STANDARD;
		return true;
	}

	struct attribute_scope_syntax *scope_match = attribute_scope(Text);
	if (!scope_match)
		return false;
	*scope = scope_match->scope;

	//TODO: If deprecated warn

	colon();
	colon();
	identifier();

	return true;
}

static struct ast_node *parse_attribute() {
	enum attribute_scope scope;

	if (!parse_attribute_scope(&scope))
		return parse_unknown_attribute();

	struct attribute_syntax *attribute_match = attribute_lookup(scope, Text);
	//TODO: If deprecated warn

	parse_attribute_invocation(attribute_match->attribute);
}

struct ast_node *parse_attribute_sequence() {
	struct ast_storage attributes = {};

	lbracket();
	lbracket();

	bool comma = true;
	while (Token.kind != T_RBRACKET) {
		if (!comma)
			fatal("expected comma after attribute");
		comma = false;

		struct ast_node *attribute = parse_attribute();
		ast_insert(&attributes, attribute);

		if (Token.kind == T_COMMA) {
			scan();
			comma = true;
		}
	}

	rbracket();
	rbracket();
}
