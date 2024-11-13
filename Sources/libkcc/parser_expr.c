//
//  parser_expr.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "parser.h"
#include "globals.h"

#include <kcc/ast.h>
#include <kcc/diagnostics.h>
#include <kcc/lexer1.h>
#include <kcc/symtable.h>

#pragma mark - Utilities

static struct ast_node *produce_unary(enum ast_kind op, struct ast_node *operand) {
	struct ast_node *node = ast_alloc();
	ast_unary_expression(node, op, operand);
	return node;
}

static struct ast_node *produce_binary(enum ast_kind op, struct ast_node *left, expression_callback callback) {
	// Consume the operator
	scan();

	struct ast_node *right = callback(true);
	if (!right) fatal("missing operand in binary expression");

	struct ast_node *node = ast_alloc();
	ast_binary_expression(node, op, left, right);
	return node;
}

#pragma mark - Primary Expression

static struct ast_node *parse_integer_expression() {
	struct ast_node *node = ast_alloc();
	struct token_integer_modifier modifier = {
		.base = BASE_NONE,
		.suffix = ISUFFIX_NONE,
		.is_unsigned = false,
	};
	ast_integer_literal(node, Integer, modifier);
	scan();
	return node;
}

static struct ast_node *parse_identifier_expression() {
	// Lookup or create the symbol
	struct symbol *symbol = symtable_find(SymbolTable, Text);
	if (!symbol) {
		symbol = symtable_insert(SymbolTable, Text);
		//TODO: Warning with diagnostics
//		fatals("Unknown variable", Text);
	}

	// Consume the identifier
	scan();

	// Produce an identifier expression
	struct ast_node *node = ast_alloc();
	ast_identifier(node, symbol);
	return node;
}

static struct ast_node *parse_parenthesized_expression() {
	// Consume the lparen
	scan();
	struct ast_node *expression = parse_expression();
	rparen();

	struct ast_node *node = ast_alloc();
	node->op = N_PARENTHESIZED_EXPRESSION;
	node->parenthesized_expression.expression = expression;
	return node;
}

/// > `generic-association`
/// > - **`type-name`** **`:`** *assignment-expression*
/// > - **`default`** **`:`** *assignment-expression*
static struct ast_node *parse_generic_association() {
	struct ast_node *key;

	if (Token.kind == T_DEFAULT)
		key = 0;
	else
		// FIXME: Actually type-name
		key = parse_assignment_expression(true);

	colon();

	struct ast_node *value = parse_assignment_expression(true);

	struct ast_node *node = ast_alloc();
	node->op = N_GENERIC_ASSOCIATION;
	node->generic_association = (struct ast_generic_association){
		.key = key,
		.value = value,
	};
	return node;
}

static struct ast_node *parse_generic_expression() {
	lparen();
	struct ast_node *control = parse_assignment_expression(true);
	comma();

	struct ast_storage associations = {};
	while (Token.kind != T_RPAREN)
		ast_insert(&associations, parse_generic_association());

	struct ast_node *node = ast_alloc();
	node->op = N_GENERIC;
	node->generic_expression = (struct ast_generic_expression){
		.control = control,
		.associations = associations,
	};
	return node;
}

static struct ast_node *parse_primary_expression(bool required) {
	switch (Token.kind) {
	case T_IDENTIFIER: return parse_identifier_expression();
	case T_INTEGER_LITERAL: return parse_integer_expression();
		//TODO: Character literal
		//TODO: String literal
	case T_LPAREN: return parse_parenthesized_expression();
	case T_GENERIC: return parse_generic_expression();
	default:
		if (required)
			fatalt("unexpected token in primary expression");
		return nullptr;
	}
}

#pragma mark - Postfix Expression

static struct ast_node *parse_call_operation(struct ast_node *operand) {
	// Consume the lparen
	scan();

	struct ast_storage arguments = {};
	bool comma = true;
	while (Token.kind != T_RPAREN) {
		if (!comma) fatal("missing comma after call argument");
		comma = false;

		ast_insert(&arguments, parse_assignment_expression(true));

		// Consume a single comma
		if (Token.kind == T_COMMA) {
			scan();
			comma = true;
		}
	}

	struct ast_node *node = ast_alloc();
	ast_call(node, operand, arguments);
	return node;
}

static struct ast_node *parse_postfix_operation(struct ast_node *operand) {
	switch (Token.kind) {
	case T_LBRACKET: return 0; //TODO: parse subscript expression
	case T_LPAREN: return parse_call_operation(operand);
	case T_PERIOD: return 0; //TODO: parse_access_expression()
	case T_ARROW: return 0; //TODO: parse_indirect_access_expression()
	case T_INCREMENT: return 0; //TODO: parse_postfix_increment_expression();
	case T_DECREMENT: return 0; //TODO: parse_postfix_decrement_expression();
		//TODO: Support compound-literal
		// ( storage-class-specifiers? type-name ) braced-initializer
	default: return nullptr;
	}
}

static struct ast_node *parse_postfix_expression(bool required) {
	struct ast_node *operand = parse_primary_expression(required);
	struct ast_node *tmp = operand;
	while (tmp) {
		operand = tmp;
		tmp = parse_postfix_operation(operand);
	}
	return operand;
}

#pragma mark - Prefix Expression

static enum ast_kind prefix_cast_operation(enum token_kind token) {
	switch (Token.kind) {
	case T_LOGICAL_AND: return N_ADDRESSOF;
	case T_ASTERISK: return N_DEREFERENCE;
	case T_PLUS: return N_POSITIVE;
	case T_SUBTRACT: return N_NEGATIVE;
	case T_TILDE: return N_BITWISE_NOT;
	case T_EXCLAIM: return N_LOGICAL_NOT;
	default: fatalt("invalid prefix expression");
	}
}

static struct ast_node *parse_prefix_expression(bool required) {
	switch (Token.kind) {
	case T_LOGICAL_AND:
	case T_ASTERISK:
	case T_PLUS:
	case T_SUBTRACT:
	case T_TILDE:
	case T_EXCLAIM:
		return produce_unary(prefix_cast_operation(Token.kind), parse_cast_expression(true));
	case T_INCREMENT: return produce_unary(N_INCREMENT_PREFIX, parse_prefix_expression(true));
	case T_DECREMENT: return produce_unary(N_DECREMENT_PREFIX, parse_prefix_expression(true));
	case T_SIZEOF: return 0; //TODO: parse_sizeof_expression();
	case T_ALIGNOF: return 0; //TODO: parse_alignof_expression();
	default: return parse_postfix_expression(required);
	}
}

#pragma mark - Cast Expression

static struct ast_node *parse_cast_operation() {
	struct ast_cast_expression params = {};
	// Consume the lparen
	scan();

	//TODO: Actually type-name
	params.type_tree = parse_identifier_expression();
	rparen();

	params.operand = parse_cast_expression(true);

	struct ast_node *node = ast_alloc();
	node->op = N_CAST;
	node->cast_expression = params;
	return node;
}

struct ast_node *parse_cast_expression(bool required) {
	switch (Token.kind) {
	case T_LPAREN: return parse_cast_operation();
	default: return parse_prefix_expression(required);
	}
}

#pragma mark - Multiplicative Expression

static enum ast_kind multiplicative_operation(enum token_kind token) {
	switch (token) {
	case T_ASTERISK: return N_MULTIPLY;
	case T_DIVIDE: return N_DIVIDE;
	case T_PERCENT: return N_MODULO;
	default: return N_UNKNOWN;
	}
}

struct ast_node *parse_multiplicative_expression(bool required) {
	struct ast_node *operand = parse_cast_expression(required);
	if (!operand) return nullptr;

	enum ast_kind kind = multiplicative_operation(Token.kind);
	if (kind == N_UNKNOWN) return operand;
	return produce_binary(kind, operand, parse_cast_expression);
}

#pragma mark - Additive Expression

struct ast_node *parse_additive_expression(bool required) {
	struct ast_node *operand = parse_multiplicative_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_PLUS: return produce_binary(N_ADD, operand, parse_multiplicative_expression);
	case T_SUBTRACT: return produce_binary(N_SUBTRACT, operand, parse_multiplicative_expression);
	default: return operand;
	}
}

#pragma mark - Shift Expression

struct ast_node *parse_shift_expression(bool required) {
	struct ast_node *operand = parse_additive_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_LSHIFT: return produce_binary(N_LSHIFT, operand, parse_additive_expression);
	case T_RSHIFT: return produce_binary(N_RSHIFT, operand, parse_additive_expression);
	default: return operand;
	}
}

#pragma mark - Relational Expression

struct ast_node *parse_relational_expression(bool required) {
	struct ast_node *operand = parse_shift_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_GT: return produce_binary(N_GT, operand, parse_shift_expression(true));
	case T_GTE: return produce_binary(N_GTE, operand, parse_shift_expression(true));
	case T_LT: return produce_binary(N_LT, operand, parse_shift_expression(true));
	case T_LTE: return produce_binary(N_LTE, operand, parse_shift_expression(true));
	default: return operand;
	}
}

#pragma mark - Equality Expression

struct ast_node *parse_equality_expression(bool required) {
	struct ast_node *operand = parse_relational_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_EQ: return produce_binary(N_EQ, operand, parse_relational_expression(true));
	case T_NEQ: return produce_binary(N_NEQ, operand, parse_relational_expression(true));
	default: return operand;
	}
}

#pragma mark - Bitwise AND Expression

struct ast_node *parse_bitwise_and_expression(bool required) {
	struct ast_node *operand = parse_equality_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_AMPERSAND: return produce_binary(N_BITWISE_AND, operand, parse_equality_expression(true));
	default: return operand;
	}
}

#pragma mark - Bitwise XOR Expression

struct ast_node *parse_bitwise_xor_expression(bool required) {
	struct ast_node *operand = parse_bitwise_and_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_CARET: return produce_binary(N_BITWISE_XOR, operand, parse_bitwise_and_expression(true));
	default: return operand;
	}
}

#pragma mark - Bitwise OR Expression

struct ast_node *parse_bitwise_or_expression(bool required) {
	struct ast_node *operand = parse_bitwise_xor_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_PIPE: return produce_binary(N_BITWISE_OR, operand, parse_bitwise_xor_expression(true));
	default: return operand;
	}
}

#pragma mark - Logical AND Expression

struct ast_node *parse_logical_and_expression(bool required) {
	struct ast_node *operand = parse_bitwise_or_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_LOGICAL_AND: return produce_binary(N_LOGICAL_AND, operand, parse_bitwise_or_expression(true));
	default: return operand;
	}
}

#pragma mark - Logical OR Expression

struct ast_node *parse_logical_or_expression(bool required) {
	struct ast_node *operand = parse_logical_and_expression(required);
	if (!operand) return nullptr;

	switch (Token.kind) {
	case T_LOGICAL_OR: return produce_binary(N_LOGICAL_OR, operand, parse_logical_and_expression(true));
	default: return operand;
	}
}

#pragma mark - Conditional Expression

struct ast_node *parse_conditional_expression(bool required) {
	struct ast_node *condition = parse_logical_or_expression(required);

	// Only continue parsing if its a ternary operator
	if (Token.kind != T_QUESTION)
		return condition;
	scan();

	struct ast_node *true_expression = parse_expression();
	colon();
	struct ast_node *false_expression = parse_conditional_expression(true);

	struct ast_node *node = ast_alloc();
	ast_if(node, condition, true_expression, false_expression);
	node->op = N_CONDITIONAL;
	return node;
}

#pragma mark - Assignment Expression

struct ast_node *parse_assignment_expression(bool required) {
	struct ast_node *operand = parse_conditional_expression(required);
	if (!operand || operand->op == N_CONDITIONAL) return operand;

	enum ast_kind op = assignment_operation(Token.kind);
	if (op == N_UNKNOWN)
		return operand;

	return produce_binary(op, operand, parse_assignment_expression(true));
}

enum ast_kind assignment_operation(enum token_kind token) {
	switch (token) {
	case T_ASSIGN: return N_ASSIGN;
	case T_ASSIGN_MULTIPLY: return N_ASSIGN_MULTIPLY;
	case T_ASSIGN_DIVIDE: return N_ASSIGN_DIVIDE;
	case T_ASSIGN_MODULO: return N_ASSIGN_MODULO;
	case T_ASSIGN_ADD: return N_ASSIGN_ADD;
	case T_ASSIGN_SUBTRACT: return N_ASSIGN_SUBTRACT;
	case T_ASSIGN_LSHIFT: return N_ASSIGN_LSHIFT;
	case T_ASSIGN_RSHIFT: return N_ASSIGN_RSHIFT;
	case T_ASSIGN_BITWISE_AND: return N_ASSIGN_BITWISE_AND;
	case T_ASSIGN_BITWISE_XOR: return N_ASSIGN_BITWISE_XOR;
	case T_ASSIGN_BITWISE_OR: return N_ASSIGN_BITWISE_OR;
	default: return N_UNKNOWN;
	}
}

#pragma mark - Comma Expression

struct ast_node *parse_comma_operation(struct ast_node *operand) {
	// Consume the comma
	scan();
	struct ast_node *value = parse_assignment_expression(true);

	struct ast_node *node = ast_alloc();
	node->op = N_COMMA;
	node->comma_expression.discarded = operand;
	node->comma_expression.returned = value;
	return node;
}

#pragma mark - Expression

struct ast_node *parse_expression() {
	struct ast_node *expression = parse_assignment_expression(true);
	if (Token.kind == T_COMMA)
		return parse_comma_operation(expression);
	return expression;
}

struct ast_node *parse_constant_expression() {
	return parse_conditional_expression(true);
}
