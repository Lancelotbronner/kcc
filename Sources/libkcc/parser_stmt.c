//
//  parser_stmt.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/diagnostics.h>
#include <kcc/scanner.h>
#include <kcc/symtable.h>

struct ast_node *parse_statements() {
	struct ast_node *tree = parse_statement();
	struct ast_node *node, *tmp;

	while ((node = parse_statement())) {
		// If we already had a node, turn it into a list
		if (tree->op != N_STATEMENTS) {
			tmp = tree;
			tree = ast_alloc();
			tree->op = N_STATEMENTS;
			tree->compound.contents.length = 0;
			ast_insert(&tree->compound.contents, tmp);
		}
		ast_insert(&tree->compound.contents, node);
	}

	return tree;
}

struct ast_node *parse_statement() {
	switch (Token.kind) {
		//TODO: T_INT is a declaration, which isn't included in statements
	case T_INT: return parse_var_declaration();
	case T_IDENTIFIER: return parse_assignment_statement();
	case T_LCURLY: return parse_compound_statement();
	case T_EOF: return nullptr;
	default: fatalt("Unexpected token in statement");
	}
};

#pragma mark - Statement Parsers

struct ast_node *parse_compound_statement() {
	struct ast_node *tree, *node;

	// Require a left curly bracket
	lbrace();

	// Prepare our storage
	tree = ast_alloc();
	ast_compound(tree);

	while (true) {
		if (Token.kind == T_RCURLY) {
			scan();
			return tree;
		}

		node = parse_statement();
		if (!node) break;
		ast_insert(&tree->compound.contents, node);
	}

	return tree;
}

struct ast_node *parse_if_statement() {
	struct ast_node *condition, *then_tree, *else_tree = NULL;

	match(T_IF, "if");

	lparen();
	condition = parse_expression();
	rparen();

	then_tree = parse_statement();

	if (Token.kind == T_ELSE) {
		scan();
		else_tree = parse_statement();
	}

	struct ast_node *node = ast_alloc();
	ast_if(node, condition, then_tree, else_tree);
	return node;
}

struct ast_node *parse_while_statement() {
	struct ast_node *condition, *loop = NULL;

	match(T_WHILE, "while");

	lparen();
	condition = parse_expression();
	rparen();

	loop = parse_statement();

	struct ast_node *node = ast_alloc();
	ast_while(node, condition, loop);
	return node;
}

struct ast_node *parse_for_statement() {
	struct ast_node *declaration, *preop, *condition, *postop, *loop = NULL;

	match(T_FOR, "for");
	lparen();

	//TODO: parse the declaration
	// If we have a declaration, preop becomes mandatory
	declaration = nullptr;
	preop = parse_expression();
	semi();

	condition = parse_expression();
	semi();

	postop = parse_expression();

	rparen();
	loop = parse_statement();

	struct ast_node *node = ast_alloc();
	ast_for(node, declaration, preop, condition, postop, loop);
	return node;
}

struct ast_node *parse_var_declaration() {
	struct symbol *symbol;
	struct type type;

	type = parse_type();

	identifier();
	symbol = symtable_insert(SymbolTable, Text);
	symbol->type = type;
	
	//  genglobsym(Text);
	semi();

	return nullptr;
}

struct ast_node *parse_assignment_statement() {
	struct ast_node *left, *right, *tree;
	struct symbol *symbol;

	// Ensure we have an identifier
	identifier();

	// Check it's been defined then make a leaf node for it
	if ((symbol = symtable_find(SymbolTable,Text))) {
		fatals("Undeclared variable", Text);
	}

	right = ast_alloc();
	ast_identifier(right, symbol);
	right->op = N_LVIDENT;

	// Ensure we have an equals sign
	match(T_ASSIGN, "=");

	// Parse the following expression
	left = parse_expression();

	// Make an assignment AST tree
	tree = ast_alloc();
	ast_binary_expression(tree, N_ASSIGN, left, right);

	// Generate the assembly code for the assignment
	//	genAST(tree, -1);
	//	genfreeregs();

	// Match the following semicolon
	semi();

	return nullptr;
}
