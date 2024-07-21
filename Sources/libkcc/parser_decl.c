//
//  parser_decl.c
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/diagnostics.h>
#include <kcc/scanner.h>
#include <kcc/symtable.h>

static enum type_kind _parse_single_type() {
	switch (Token.kind) {
	case T_CHAR: return TYP_CHAR;
	case T_INT: return TYP_INT;
	case T_VOID: return TYP_VOID;
	default: return TYP_UNKNOWN;
	}
}

struct type parse_type() {
	struct type type;

	type.kind = _parse_single_type();
	if (type.kind != TYP_UNKNOWN) {
		scan();
		return type;
	}

	fatald("Illegal type, token", Token.kind);
}

//TODO: See if there isn't a better structure for declarators
// The current one seems to rely a LOT on semantics for everything
// Maybe see how Swift does it?

struct ast_node *parse_declaration() {
	struct ast_node *node;
	struct type type;

	while (true) {
		// Function and variable declaration both share a type and identifier
		type = parse_type();
		identifier();

		switch (Token.kind) {
		case T_LPAREN:
			return parse_function_declaration(type);
		case T_COMMA:
		case T_SEMICOLON:
			return parse_var_declaration();
		default:
			break;
		}
	}
}

struct ast_node *parse_declarator(struct type type) {
	switch (Token.kind) {
	case T_ASTERISK: return parse_pointer_declarator(type);
	case T_LPAREN: return parse_grouped_declarator(type);
	case T_IDENTIFIER: return parse_identifier_declarator(type);
	default: fatal("Invalid token in declarator");
	}
}

struct ast_node *parse_pointer_declarator(struct type type) {
	struct type pointer_type;

	// Consume the `*` and transform the type
	scan();
	pointer_type = type_pointer(type);

	//TODO: parse optional attribute sequence
	//TODO: parse optional type qualifiers

//	struct ast_node *node = ast_alloc();
//	ast_pointer_declarator(node, declarator);
//	return node;
	return 0;
}

struct ast_node *parse_grouped_declarator(struct type type) {
	lparen();
	struct ast_node *declarator = parse_declarator(type);
	rparen();

	struct ast_node *node = ast_alloc();
	ast_group_declarator(node, declarator);
	return declarator;
}

struct ast_node *parse_identifier_declarator(struct type type) {
	// Declare the associated symbol
	struct symbol *symbol = symtable_insert(SymbolTable, Text);

	// Consume the identifier
	scan();

	//TODO: parse optional attribute sequence

	struct ast_node *node = ast_alloc();
	ast_identifier_declarator(node, type, symbol);
	return node;
}

struct ast_node *parse_function_declarator(struct ast_node *declaration) {
	// Consume the `(`
	scan();
	//TODO: parse function parameter declarations
	rparen();

	struct ast_node *node = ast_alloc();
	ast_function_declarator(node, declaration);
	return node;
}

struct ast_node *parse_function_declaration(struct type type) {
	// assume the identifier was just parsed
	struct ast_node *definition;
	struct symbol *symbol;

	symbol = symtable_insert(SymbolTable, Text);
	lparen();
	rparen();

	// Get the AST tree for the compound statement
	definition = parse_compound_statement();

	// Return an N_FUNCTION node which has the function's nameslot
	// and the compound statement sub-tree
	struct ast_node *node = ast_alloc();
	ast_function(node, symbol, definition);

	//			genAST(tree, NOREG, 0);

	return node;
}

struct ast_node *parse_variable_declaration(struct type type) {
	// assume the identifier was just parsed
	struct ast_node *definition;
	struct symbol *symbol;

	symbol = symtable_insert(SymbolTable, Text);
	lparen();
	rparen();

	// Get the AST tree for the compound statement
	definition = parse_compound_statement();

	// Return an N_FUNCTION node which has the function's nameslot
	// and the compound statement sub-tree
	struct ast_node *node = ast_alloc();
	ast_function(node, symbol, definition);
	return node;
}
