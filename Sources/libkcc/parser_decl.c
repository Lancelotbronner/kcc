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
#include <kcc/type_builder.h>

static enum type_kind _parse_single_type() {
	switch (Token.kind) {
	case T_CHAR: return TYP_CHAR;
	case T_INT: return TYP_INT;
	case T_SHORT: return TYP_SHORT;
	case T_VOID: return TYP_VOID;
	default: return TYP_UNKNOWN;
	}
}

static bool parse_declaration_specifier(parser_t parser) {
	switch (Token.kind) {
		// Storage Class
	case T_AUTO:
		parser->specifiers.is_auto = true;
		return true;
	case kconstexpr:
		parser->specifiers.is_constexpr = true;
		return true;
	case kextern:
		parser->specifiers.is_extern = true;
		return true;
	case kregister:
		parser->specifiers.is_register = true;
		return true;

		// Function Specifiers
	case T_NORETURN:
		parser->specifiers.is_noreturn = true;
		return true;
	case T_INLINE:
		parser->specifiers.is_inline = true;
		return true;

		// Type Specifier Qualifier
	case T_VOID:
//		tbuilder_void(builder);
		return true;
//		void
//		char
//		short
//		int
//		long
//		float
//		double
//		signed
//		unsigned
//		_BitInt ( constant-expression )
//		bool
//		_Complex
//		_Decimal32
//		_Decimal64
//		_Decimal128
//		atomic-type-specifier
//		struct-or-union-specifier
//		enum-specifier
//		typedef-name
//		typeof-specifier

//		type-specifier-qualifier
	}
}

ast_t parse_declaration_specifiers(parser_t parser) {
//	declaration-specifier attribute-specifier-sequenceopt
//	declaration-specifier declaration-specifiers

	//TODO: parse any kind of specifier, constructing AST nodes and the type
	//TODO: parse an optional attribute sequence
	//TODO: return an AST node

	//TODO: Detect duplicate specifiers
	if (parse_declaration_specifier(parser))
		lexer_advance(parser->lexer);

	struct type type;
	type.kind = _parse_single_type();
	if (type.kind != TYP_UNKNOWN) {
		lexer_advance(Lexer);
		return nullptr;
	}

	fatalt("Illegal type");
}

ast_t parse_function_declaration(parser_t parser, struct type type) {
	// assume the identifier was just parsed
	ast_t definition;
	struct symbol *symbol;

	symbol = symtable_insert(SymbolTable, TokenSource);
	lparen();
	rparen();

	// Get the AST tree for the compound statement
	definition = parse_compound_statement(parser);

	// Return an N_FUNCTION node which has the function's nameslot
	// and the compound statement sub-tree
	ast_t node = ast_alloc();
	ast_function(node, symbol, nullptr, definition);

	//			genAST(tree, NOREG, 0);

	return node;
}

ast_t parse_variable_declaration(parser_t parser, struct type type) {
	// assume the identifier was just parsed
	ast_t definition;
	struct symbol *symbol;

	symbol = symtable_insert(SymbolTable, TokenSource);
	lparen();
	rparen();

	// Get the AST tree for the compound statement
	definition = parse_compound_statement(parser);

	// Return an N_FUNCTION node which has the function's nameslot
	// and the compound statement sub-tree
	ast_t node = ast_alloc();
	ast_function(node, symbol, nullptr, definition);
	return node;
}

#pragma mark - Initializer Parsing

//TODO: Move initializer parsing to parse_init.c
ast_t parse_initializer(parser_t parser) {
	return parse_expression(parser);
}

#pragma mark - Declarator Parsing

static ast_t parse_pointer_declarator(parser_t parser, struct type *type) {
	// Consume the `*` and transform the type
	lexer_advance(Lexer);
	*type = type_pointer(type);

	//TODO: parse optional attribute sequence
	//TODO: parse optional type qualifiers

	ast_t declarator = parse_declarator(parser, *type);

	ast_t node = ast_alloc();
	ast_pointer_declarator(node, declarator);
	return node;
}

static ast_t parse_grouped_declarator(parser_t parser, struct type *type) {
	lparen();
	ast_t declarator = parse_declarator(parser, *type);
	rparen();

	ast_t node = ast_alloc();
	ast_group_declarator(node, declarator);
	return declarator;
}

static ast_t parse_identifier_declarator(parser_t parser, struct type *type) {
	// Declare the associated symbol
	Symbol = symtable_insert(SymbolTable, TokenSource);

	// Consume the identifier
	lexer_advance(Lexer);

	//TODO: parse optional attribute sequence

	ast_t node = ast_alloc();
	ast_identifier_declarator(node, *type, Symbol);
	return node;
}

static ast_t parse_declarator_prefix(parser_t parser, struct type *type) {
	switch (Token.kind) {
	case T_ASTERISK: return parse_pointer_declarator(parser, type);
	case T_LPAREN: return parse_grouped_declarator(parser, type);
	case T_IDENTIFIER: return parse_identifier_declarator(parser, type);
	default: return nullptr;
	}
}

static ast_t parse_array_declarator(parser_t parser, struct type *type, ast_t declarator) {
	// Consume the `[`
	lexer_advance(Lexer);
	//TODO: parse type qualifiers
	//TODO: parse static
	//TODO: parse variable length *
	ast_t size = parse_assignment_expression(parser, true);
	rbracket();

	//TODO: Modify the type

	ast_t node = ast_alloc();
	ast_array_declarator(node, declarator, size);
	return node;
}

static ast_t parse_function_declarator(struct type *type, ast_t declarator) {
	// Consume the `(`
	lexer_advance(Lexer);
	//TODO: parse function parameter declarations
	rparen();

	//TODO: Modify the type

	ast_t node = ast_alloc();
	ast_function_declarator(node, declarator);
	return node;
}

static ast_t parse_declarator_postfix(parser_t parser, struct type *type, ast_t declarator) {
	switch (Token.kind) {
	case T_LBRACKET: return parse_array_declarator(parser, type, declarator);
	case T_LPAREN: return parse_function_declarator(type, declarator);
	default: return nullptr;
	}
}

ast_t parse_declarator(parser_t parser, struct type type) {
	ast_t declarator = parse_declarator_prefix(parser, &type);

	ast_t tmp = declarator;
	while (tmp) {
		declarator = tmp;
		tmp = parse_declarator_postfix(parser, &type, declarator);
	}

	return declarator;
}

#pragma mark - Declaration Parsing

static ast_t parse_function_definition(parser_t parser, ast_t declarator) {
	if (declarator->op != N_FUNCTION_DECLARATOR)
		return nullptr;

	ast_t body = parse_compound_statement(parser);

	ast_t node = ast_alloc();
	ast_function(node, declarator->declarator.symbol, declarator, body);
	return node;
}

static ast_t parse_init_declarator(parser_t parser, ast_t declarator) {
	// init clauses start with an equals
	if (!lexer_match(Lexer, T_ASSIGN))
		return nullptr;
	return parse_initializer(parser);
}

ast_t parse_declaration(parser_t parser, bool isFunctionDefinitionAllowed) {
	// check early for satic_assert as it doesn't support general declarations
	if (lexer_match(Lexer, kstatic_assert))
		fatal("static_assert declarations aren't implemented");

	// attribute sequences are a valid declaration
	ast_t attributes = parse_attribute_sequence(parser);
	if (lexer_match(Lexer, T_SEMICOLON))
		fatal("attribute declaration aren't implemented");

	// declaration specifiers are a valid declaration
	parser->type.kind = TYP_UNKNOWN;
	ast_t specifiers = parse_declaration_specifiers(parser);
	if (lexer_match(Lexer, T_SEMICOLON)) {
		if (attributes) {
			// unless you have prefix attribute??
			//TODO: diagnostic error: declaration with prefix attributes requires declarators [fix-it: move to suffix]
		}
		fatal("specifiers declaration aren't implemented");
	}

	// try parsing function definitions if requested
	ast_t declarator = parse_declarator(parser, Type);
	ast_t definition = isFunctionDefinitionAllowed && parse_function_definition(parser, declarator);
	if (definition)
		return definition;

	// else finish parsing the declarator
	declarator->declarator.initializer = parse_init_declarator(parser, declarator);

	// parse remaining declarators with optional initalizers
	//TODO: find a way to abstract the parsing for these comma lists? maybe iterator pattern?
	struct ast_storage declarators = {};
	ast_insert(&declarators, declarator);
	bool comma = true;
	while (!lexer_match(Lexer, T_SEMICOLON)) {
		if (!comma)
			fatal("expected comma after declarator");
		comma = false;

		ast_t declarator = parse_declarator(parser, Type);
		declarator->declarator.initializer = parse_init_declarator(parser, declarator);
		ast_insert(&declarators, declarator);

		if (Token.kind == T_COMMA) {
			lexer_advance(Lexer);
			comma = true;
		}
	}

	ast_t node = ast_alloc();
	ast_declaration(node, Type, declarators);
	return node;
}
