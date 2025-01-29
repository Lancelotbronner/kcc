//
//  print_ast.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include "print.h"

#include <stdio.h>

typedef void (*print_callback)(ast_t node);

void print_node(struct tprinter *printer, ast_t node, char *name, bool leaf) {
	if (!node) return;
	tprinter_item(printer, leaf);
	if (!print_header(node, name))
		return;
	print_node_summary(node);
	putchar('\n');
	tprinter_indent(printer);
	print_node_body(printer, node);
	tprinter_unindent(printer);
}

void print_nodes(struct tprinter *printer, struct ast_storage *nodes, bool leaf) {
	if (ast_empty(nodes)) return;
	size_t length = ast_length(nodes);

	if (leaf)
		for (int i = 0; i < length; i++)
			print_node(printer, ast_at(nodes, i), "", i == length - 1);
	else
		for (int i = 0; i < length; i++)
			print_node(printer, ast_at(nodes, i), "", false);
}

#pragma mark - AST Summary Printing

void print_node_summary(ast_t node) {
	switch (node->op) {
		// Translation Unit
	case N_UNIT:
		//TODO: print filename?
		print_name(node);
		break;
		// Literal Expressions
	case N_INTEGER_LITERAL:
		printf("%zu", node->integer_literal.value);
		break;
		// Declarations
	case N_DECLARATION:
		//TODO: print base type?
		printf("%zu declarator%s", node->declaration.declarators.length, node->declaration.declarators.length > 1 ? "s" : "");
		break;
	case N_DECLARATOR:
		printf("%s %s", "<type>", node->declarator.symbol->name);
		break;
	case N_IDENTIFIER_DECLARATOR:
		printf("`%s`", node->identifier_declarator.symbol->name);
		break;
		// Primary Expressions
	case N_IDENTIFIER:
		printf("`%s`", node->identifier.symbol->name);
		break;
		// Unary Expressions
	case N_ADDRESSOF:
	case N_DEREFERENCE:
	case N_POSITIVE:
	case N_NEGATIVE:
	case N_BITWISE_NOT:
	case N_LOGICAL_NOT:
	case N_INCREMENT_PREFIX:
	case N_DECREMENT_PREFIX:
		print_name(node);
		//TODO: print type
		break;
		// Binary Expressions
	case N_MULTIPLY:
	case N_DIVIDE:
	case N_MODULO:
	case N_ADD:
	case N_SUBTRACT:
	case N_LSHIFT:
	case N_RSHIFT:
	case N_GT:
	case N_GTE:
	case N_LT:
	case N_LTE:
	case N_EQ:
	case N_NEQ:
	case N_BITWISE_AND:
	case N_BITWISE_XOR:
	case N_BITWISE_OR:
	case N_LOGICAL_AND:
	case N_LOGICAL_OR:
	case N_ASSIGN:
	case N_ASSIGN_MULTIPLY:
	case N_ASSIGN_DIVIDE:
	case N_ASSIGN_MODULO:
	case N_ASSIGN_ADD:
	case N_ASSIGN_SUBTRACT:
	case N_ASSIGN_LSHIFT:
	case N_ASSIGN_RSHIFT:
	case N_ASSIGN_BITWISE_AND:
	case N_ASSIGN_BITWISE_XOR:
	case N_ASSIGN_BITWISE_OR:
		print_name(node);
		//TODO: print type
		break;
	case N_CAST:
		//TODO: print type
		printf("(%s)", node->cast_expression.type_tree->identifier.symbol->name);
		break;
	default:
		print_name(node);
		break;
	}
}

void print_node_body(struct tprinter *printer, ast_t node) {
	switch (node->op) {
	case N_CAST:
		print_node(printer, node->cast_expression.type_tree, "type", false);
		print_node(printer, node->cast_expression.operand, "", true);
		break;
	case N_IF:
	case N_CONDITIONAL:
		print_node(printer, node->if_statement.condition, "condition", false);
		print_node(printer, node->if_statement.then_tree, "then", !node->if_statement.else_tree);
		print_node(printer, node->if_statement.else_tree, "else", true);
		break;
		// Translation Unit
	case N_UNIT:
		print_nodes(printer, &node->unit.declarations, true);
		break;
		// Declarations
	case N_DECLARATION:
		print_node(printer, node->declaration.type_tree, "", ast_empty(&node->declaration.declarators));
		print_nodes(printer, &node->declaration.declarators, true);
		break;
	case N_DECLARATOR:
		print_node(printer, node->declarator.declarator, nullptr, !node->declarator.initializer);
		print_node(printer, node->declarator.initializer, nullptr, true);
		break;
	case N_POINTER_DECLARATOR:
		print_node(printer, node->pointer_declarator.declarator, nullptr, true);
		break;
	case N_GROUP_DECLARATOR:
		print_node(printer, node->group_declarator.declarator, nullptr, true);
		break;
	case N_ARRAY_DECLARATOR:
		print_node(printer, node->array_declarator.size_tree, "size", false);
		print_node(printer, node->array_declarator.declarator, nullptr, true);
		break;
	case N_FUNCTION_DECLARATOR:
		print_node(printer, node->function_declarator.declarator, nullptr, true);
		break;
		// Unary Expressions
	case N_ADDRESSOF:
	case N_DEREFERENCE:
	case N_POSITIVE:
	case N_NEGATIVE:
	case N_BITWISE_NOT:
	case N_LOGICAL_NOT:
	case N_INCREMENT_PREFIX:
	case N_DECREMENT_PREFIX:
		print_node(printer, node->unary_expression.operand, "", true);
		break;
		// Binary Expressions
	case N_MULTIPLY:
	case N_DIVIDE:
	case N_MODULO:
	case N_ADD:
	case N_SUBTRACT:
	case N_LSHIFT:
	case N_RSHIFT:
	case N_GT:
	case N_GTE:
	case N_LT:
	case N_LTE:
	case N_EQ:
	case N_NEQ:
	case N_BITWISE_AND:
	case N_BITWISE_XOR:
	case N_BITWISE_OR:
	case N_LOGICAL_AND:
	case N_LOGICAL_OR:
	case N_ASSIGN:
	case N_ASSIGN_MULTIPLY:
	case N_ASSIGN_DIVIDE:
	case N_ASSIGN_MODULO:
	case N_ASSIGN_ADD:
	case N_ASSIGN_SUBTRACT:
	case N_ASSIGN_LSHIFT:
	case N_ASSIGN_RSHIFT:
	case N_ASSIGN_BITWISE_AND:
	case N_ASSIGN_BITWISE_XOR:
	case N_ASSIGN_BITWISE_OR:
		print_node(printer, node->binary_expression.lhs, "", false);
		print_node(printer, node->binary_expression.rhs, "", true);
		break;
	default:
		break;
	}
}
