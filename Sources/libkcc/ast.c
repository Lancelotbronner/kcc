//
//  ast.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include <kcc/ast.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

struct ast_node *ast_alloc() {
	struct ast_node *n = malloc(sizeof(struct ast_node));
	if (!n) {
		fprintf(stderr, "Unable to malloc in ast_alloc()\n");
		exit(1);
	}
	return n;
}

#pragma mark - Node Constructors

void ast_binary_expression(struct ast_node *node, enum ast_kind op, struct ast_node *left, struct ast_node *right) {
	node->op = op;
	node->binary_expression.lhs = left;
	node->binary_expression.rhs = right;

	struct type left_type = ast_type(left);
	struct type right_type = ast_type(right);
	struct type result_type = type_promote(left_type, right_type);
	if (result_type.kind == TYP_UNKNOWN)
		result_type = type_promote(right_type, left_type);
	node->binary_expression.type = result_type;
}

void ast_unary_expression(struct ast_node *node, enum ast_kind op, struct ast_node *operand) {
	node->op = op;
	node->unary_expression.type = ast_type(operand);
	node->unary_expression.operand = operand;
}

void ast_integer_literal(struct ast_node *node, size_t value, enum token_base_modifier base, enum token_integer_modifier suffix) {
	node->op = N_INTEGER_LITERAL;
	node->integer_literal.value = value;
	node->integer_literal.base = base;
	node->integer_literal.modifier = suffix;
}

void ast_identifier(struct ast_node *node, struct symbol *symbol) {
	node->op = N_IDENTIFIER;
	node->identifier.symbol = symbol;
}

void ast_compound(struct ast_node *node) {
	node->op = N_COMPOUND;
}

void ast_if(struct ast_node *node, struct ast_node *condition, struct ast_node *then_tree, struct ast_node *else_tree) {
	node->op = N_IF;
	node->if_statement.condition = condition;
	node->if_statement.then_tree = then_tree;
	node->if_statement.else_tree = else_tree;
}

void ast_for(struct ast_node *node, struct ast_node *declaration, struct ast_node *preop, struct ast_node *condition, struct ast_node *postop, struct ast_node *loop) {
	node->op = N_FOR;
	node->for_statement.declaration = declaration;
	node->for_statement.preop = preop;
	node->for_statement.condition = condition;
	node->for_statement.postop = postop;
	node->for_statement.loop = loop;
}

void ast_while(struct ast_node *node, struct ast_node *condition, struct ast_node *loop) {
	node->op = N_WHILE;
	node->while_statement.condition = condition;
	node->while_statement.loop = loop;
}

void ast_call(struct ast_node *node, struct ast_node *function, struct ast_storage arguments) {
	node->op = N_CALL;
	node->call_expression.operand = function;
	node->call_expression.arguments = arguments;
}

void ast_function(struct ast_node *node, struct symbol *symbol, struct ast_node *definition) {
	node->op = N_FUNCTION;
	node->function_declaration.symbol = symbol;
	node->function_declaration.definition = definition;
}

void ast_declaration(struct ast_node *node, struct type type, struct ast_storage declarators) {
	node->op = N_DECLARATION;
	node->declaration.type = type;
	node->declaration.declarators = declarators;
}

void ast_declarator(struct ast_node *node, struct symbol *symbol, struct type type, struct ast_node *declarator, struct ast_node *initializer) {
	node->op = N_DECLARATOR;
	node->declarator.symbol = symbol;
	node->declarator.type = type;
	node->declarator.declarator = declarator;
	node->declarator.initializer = initializer;
}

void ast_pointer_declarator(struct ast_node *node, struct ast_node *declarator) {
	node->op = N_POINTER_DECLARATOR;
	node->pointer_declarator.declarator = declarator;
}

void ast_group_declarator(struct ast_node *node, struct ast_node *declarator) {
	node->op = N_GROUP_DECLARATOR;
	node->group_declarator.declarator = declarator;
}

void ast_identifier_declarator(struct ast_node *node, struct type type, struct symbol *symbol) {
	node->op = N_IDENTIFIER_DECLARATOR;
	node->identifier_declarator.symbol = symbol;
}

void ast_array_declarator(struct ast_node *node, struct ast_node *declarator, struct ast_node *size) {
	node->op = N_ARRAY_DECLARATOR;
	node->array_declarator.declarator = declarator;
	node->array_declarator.size_tree = size;
}

void ast_function_declarator(struct ast_node *node, struct ast_node *declarator) {
	node->op = N_FUNCTION_DECLARATOR;
	node->function_declarator.declarator = declarator;
}

#pragma mark - Node Properties

struct type ast_type(struct ast_node *node) {
	switch (node->op) {
	case N_INTEGER_LITERAL:
		return type_init(TYP_INT);
	case N_ADD:
	case N_SUBTRACT:
	case N_MULTIPLY:
	case N_DIVIDE:
		return node->binary_expression.type;
	case N_ADDRESSOF:
	case N_DEREFERENCE:
		return node->unary_expression.type;
	default:
		return type_init(TYP_UNKNOWN);
	}
}
