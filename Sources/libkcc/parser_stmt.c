//
//  parser_stmt.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/diagnostics.h>
#include <kcc/lexer.h>
#include <kcc/symtable.h>

ast_t parse_statements(parser_t parser) {
	ast_t tree = parse_statement(parser);
	ast_t node, tmp;

	while ((node = parse_statement(parser))) {
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

ast_t parse_statement(parser_t parser) {
	switch (Token.kind) {
		//TODO: T_INT is a declaration, which isn't included in statements
	case T_INT: return parse_var_declaration(parser);
	case T_IDENTIFIER: return parse_assignment_statement(parser);
	case T_LCURLY: return parse_compound_statement(parser);
	case T_EOF: return nullptr;
	default: fatalt("Unexpected token in statement");
	}
};

#pragma mark - Statement Parsers

ast_t parse_compound_statement(parser_t parser) {
	ast_t tree, node;

	// Require a left curly bracket
	lbrace();

	// Prepare our storage
	tree = ast_alloc();
	ast_compound(tree);

	while (true) {
		if (Token.kind == T_RCURLY) {
			lexer_advance(Lexer);
			return tree;
		}

		node = parse_statement(parser);
		if (!node) break;
		ast_insert(&tree->compound.contents, node);
	}

	return tree;
}

ast_t parse_if_statement(parser_t parser) {
	ast_t condition, then_tree, else_tree = NULL;

	match(T_IF, "if");

	lparen();
	condition = parse_expression(parser);
	rparen();

	then_tree = parse_statement(parser);

	if (Token.kind == T_ELSE) {
		lexer_advance(Lexer);
		else_tree = parse_statement(parser);
	}

	ast_t node = ast_alloc();
	ast_if(node, condition, then_tree, else_tree);
	return node;
}

ast_t parse_while_statement(parser_t parser) {
	ast_t condition, loop = NULL;

	match(T_WHILE, "while");

	lparen();
	condition = parse_expression(parser);
	rparen();

	loop = parse_statement(parser);

	ast_t node = ast_alloc();
	ast_while(node, condition, loop);
	return node;
}

ast_t parse_for_statement(parser_t parser) {
	ast_t declaration, preop, condition, postop, loop = NULL;

	match(T_FOR, "for");
	lparen();

	//TODO: parse the declaration
	// If we have a declaration, preop becomes mandatory
	declaration = nullptr;
	preop = parse_expression(parser);
	semi();

	condition = parse_expression(parser);
	semi();

	postop = parse_expression(parser);

	rparen();
	loop = parse_statement(parser);

	ast_t node = ast_alloc();
	ast_for(node, declaration, preop, condition, postop, loop);
	return node;
}

ast_t parse_var_declaration(parser_t parser) {
	struct symbol *symbol;
	struct type type;

//	type = parse_type();

	identifier();
	symbol = symtable_insert(SymbolTable, TokenSource);
	symbol->type = type;
	
	//  genglobsym(TokenSource);
	semi();

	return nullptr;
}

ast_t parse_assignment_statement(parser_t parser) {
	ast_t left, right, tree;
	struct symbol *symbol;

	// Ensure we have an identifier
	identifier();

	// Check it's been defined then make a leaf node for it
	if ((symbol = symtable_find(SymbolTable,TokenSource))) {
		fatals("Undeclared variable", TokenSource);
	}

	right = ast_alloc();
	ast_identifier(right, symbol);
	right->op = N_LVIDENT;

	// Ensure we have an equals sign
	match(T_ASSIGN, "=");

	// Parse the following expression
	left = parse_expression(parser);

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
