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

ast_t ast_alloc() {
	ast_t n = malloc(sizeof(struct ast_node));
	if (!n) {
		fprintf(stderr, "Unable to malloc in ast_alloc()\n");
		exit(1);
	}
	return n;
}

#pragma mark - Node Constructors

void ast_unit(ast_t node, struct ast_storage declarations) {
	node->op = N_UNIT;
	node->unit.declarations = declarations;
}

void ast_binary_expression(ast_t node, enum ast_kind op, ast_t left, ast_t right) {
	node->op = op;
	node->binary_expression.lhs = left;
	node->binary_expression.rhs = right;

	struct type left_type = ast_type(left);
	struct type right_type = ast_type(right);
	type_t result_type = type_promote(&left_type, &right_type);
	if (result_type->kind == TYP_UNKNOWN)
		result_type = type_promote(&right_type, &left_type);
	node->binary_expression.type = *result_type;
}

void ast_unary_expression(ast_t node, enum ast_kind op, ast_t operand) {
	node->op = op;
	node->unary_expression.type = ast_type(operand);
	node->unary_expression.operand = operand;
}

void ast_integer_literal(ast_t node, size_t value, struct token_integer_modifier modifier) {
	node->op = N_INTEGER_LITERAL;
	node->integer_literal.value = value;
	node->integer_literal.modifier = modifier;
}

void ast_identifier(ast_t node, struct symbol *symbol) {
	node->op = N_IDENTIFIER;
	node->identifier.symbol = symbol;
}

void ast_compound(ast_t node) {
	node->op = N_COMPOUND;
}

void ast_if(ast_t node, ast_t condition, ast_t then_tree, ast_t else_tree) {
	node->op = N_IF;
	node->if_statement.condition = condition;
	node->if_statement.then_tree = then_tree;
	node->if_statement.else_tree = else_tree;
}

void ast_for(ast_t node, ast_t declaration, ast_t preop, ast_t condition, ast_t postop, ast_t loop) {
	node->op = N_FOR;
	node->for_statement.declaration = declaration;
	node->for_statement.preop = preop;
	node->for_statement.condition = condition;
	node->for_statement.postop = postop;
	node->for_statement.loop = loop;
}

void ast_while(ast_t node, ast_t condition, ast_t loop) {
	node->op = N_WHILE;
	node->while_statement.condition = condition;
	node->while_statement.loop = loop;
}

void ast_call(ast_t node, ast_t function, struct ast_storage arguments) {
	node->op = N_CALL;
	node->call_expression.operand = function;
	node->call_expression.arguments = arguments;
}

void ast_function(ast_t node, struct symbol *symbol, ast_t declaration, ast_t definition) {
	node->op = N_FUNCTION;
	node->function_declaration.symbol = symbol;
	node->function_declaration.declaration = declaration;
	node->function_declaration.definition = definition;
}

void ast_declaration(ast_t node, struct type type, struct ast_storage declarators) {
	node->op = N_DECLARATION;
	node->declaration.type = type;
	node->declaration.declarators = declarators;
}

void ast_declarator(ast_t node, struct symbol *symbol, struct type type, ast_t declarator, ast_t initializer) {
	node->op = N_DECLARATOR;
	node->declarator.symbol = symbol;
	node->declarator.type = type;
	node->declarator.declarator = declarator;
	node->declarator.initializer = initializer;
}

void ast_pointer_declarator(ast_t node, ast_t declarator) {
	node->op = N_POINTER_DECLARATOR;
	node->pointer_declarator.declarator = declarator;
}

void ast_group_declarator(ast_t node, ast_t declarator) {
	node->op = N_GROUP_DECLARATOR;
	node->group_declarator.declarator = declarator;
}

void ast_identifier_declarator(ast_t node, struct type type, struct symbol *symbol) {
	node->op = N_IDENTIFIER_DECLARATOR;
	node->identifier_declarator.symbol = symbol;
}

void ast_array_declarator(ast_t node, ast_t declarator, ast_t size) {
	node->op = N_ARRAY_DECLARATOR;
	node->array_declarator.declarator = declarator;
	node->array_declarator.size_tree = size;
}

void ast_function_declarator(ast_t node, ast_t declarator) {
	node->op = N_FUNCTION_DECLARATOR;
	node->function_declarator.declarator = declarator;
}

#pragma mark - Node Properties

struct type ast_type(ast_t node) {
	switch (node->op) {
	case N_INTEGER_LITERAL:
		return *type_int();
	case N_ADD:
	case N_SUBTRACT:
	case N_MULTIPLY:
	case N_DIVIDE:
		return node->binary_expression.type;
	case N_ADDRESSOF:
	case N_DEREFERENCE:
		return node->unary_expression.type;
	default:
		return *type_unknown();
	}
}
