//
//  parser_decl.c
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/diagnostics.h>
#include <kcc/lexer.h>
#include <kcc/symtable.h>

static enum type_kind _parse_single_type() {
	switch (Token.kind) {
	case T_CHAR: return TYP_CHAR;
	case T_INT: return TYP_INT;
	case T_SHORT: return TYP_SHORT;
	case T_VOID: return TYP_VOID;
	default: return TYP_UNKNOWN;
	}
}

static struct type parse_declaration_specifiers() {
	struct type type;

	type.kind = _parse_single_type();
	if (type.kind != TYP_UNKNOWN) {
		lexer_advance();
		return type;
	}

	fatalt("Illegal type");
}

struct ast_node *parse_function_declaration(struct type type) {
	// assume the identifier was just parsed
	struct ast_node *definition;
	struct symbol *symbol;

	symbol = symtable_insert(SymbolTable, TokenSource);
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

	symbol = symtable_insert(SymbolTable, TokenSource);
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

#pragma mark - Initializer Parsing

//TODO: Move initializer parsing to parse_init.c
struct ast_node *parse_initializer() {
	return parse_expression();
}

#pragma mark - Declarator Parsing

static struct ast_node *parse_pointer_declarator(struct type *type) {
	// Consume the `*` and transform the type
	lexer_advance();
	*type = type_pointer(*type);

	//TODO: parse optional attribute sequence
	//TODO: parse optional type qualifiers

	struct ast_node *declarator = parse_declarator(*type);

	struct ast_node *node = ast_alloc();
	ast_pointer_declarator(node, declarator);
	return node;
}

static struct ast_node *parse_grouped_declarator(struct type *type) {
	lparen();
	struct ast_node *declarator = parse_declarator(*type);
	rparen();

	struct ast_node *node = ast_alloc();
	ast_group_declarator(node, declarator);
	return declarator;
}

static struct ast_node *parse_identifier_declarator(struct type *type) {
	// Declare the associated symbol
	Symbol = symtable_insert(SymbolTable, TokenSource);

	// Consume the identifier
	lexer_advance();

	//TODO: parse optional attribute sequence

	struct ast_node *node = ast_alloc();
	ast_identifier_declarator(node, *type, Symbol);
	return node;
}

static struct ast_node *parse_declarator_prefix(struct type *type) {
	switch (Token.kind) {
	case T_ASTERISK: return parse_pointer_declarator(type);
	case T_LPAREN: return parse_grouped_declarator(type);
	case T_IDENTIFIER: return parse_identifier_declarator(type);
	default: return nullptr;
	}
}

static struct ast_node *parse_array_declarator(struct type *type, struct ast_node *declarator) {
	// Consume the `[`
	lexer_advance();
	//TODO: parse type qualifiers
	//TODO: parse static
	//TODO: parse variable length *
	struct ast_node *size = parse_assignment_expression(true);
	rbracket();

	//TODO: Modify the type

	struct ast_node *node = ast_alloc();
	ast_array_declarator(node, declarator, size);
	return node;
}

static struct ast_node *parse_function_declarator(struct type *type, struct ast_node *declarator) {
	// Consume the `(`
	lexer_advance();
	//TODO: parse function parameter declarations
	rparen();

	//TODO: Modify the type

	struct ast_node *node = ast_alloc();
	ast_function_declarator(node, declarator);
	return node;
}

static struct ast_node *parse_declarator_postfix(struct type *type, struct ast_node *declarator) {
	switch (Token.kind) {
	case T_LBRACKET: return parse_array_declarator(type, declarator);
	case T_LPAREN: return parse_function_declarator(type, declarator);
	default: return nullptr;
	}
}

struct ast_node *parse_declarator(struct type type) {
	struct ast_node *declarator = parse_declarator_prefix(&type);

	struct ast_node *tmp = declarator;
	while (tmp) {
		declarator = tmp;
		tmp = parse_declarator_postfix(&type, declarator);
	}

	return declarator;
}

#pragma mark - Declaration Parsing

static struct ast_node *parse_declaration_declarator(struct type type) {
	struct ast_node *declarator = parse_declarator(type);

	struct ast_node *initializer = nullptr;
	if (Token.kind == T_ASSIGN) {
		lexer_advance();
		initializer = parse_initializer();
	}

	struct ast_node *node = ast_alloc();
	ast_declarator(node, Symbol, type, declarator, initializer);
	return node;
}

struct ast_node *parse_declaration() {
	struct ast_storage declarators = {};
	struct type type = parse_declaration_specifiers();

	bool comma = true;
	while (Token.kind != T_SEMICOLON) {
		if (!comma)
			fatal("expected comma after declarator");
		comma = false;

		struct ast_node *declarator = parse_declaration_declarator(type);
		ast_insert(&declarators, declarator);

		if (Token.kind == T_COMMA) {
			lexer_advance();
			comma = true;
		}
	}

	semi();

	struct ast_node *node = ast_alloc();
	ast_declaration(node, type, declarators);
	return node;
}
