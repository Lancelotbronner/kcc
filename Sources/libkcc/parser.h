//
//  parser.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <kcc/parser.h>

#include <kcc/ast_node.h>
#include <kcc/specifiers.h>
#include <kcc/token_kind.h>
#include <kcc/type.h>

struct parser {
	lexer_t lexer;

	// TODO: store (optional) current attributes

	/// Temporary storage for AST nodes.
	struct ast_storage storage;

	/// The current type
	struct type type;

	//TODO: store alignment qualifier

	enum parser_mode mode : 1;

	struct specifiers specifiers;
};

struct type Type;
struct symbol *Symbol;

ast_t parse_unit(parser_t parser);

void check_specifier(bool is_specifier);

#pragma mark - Attribute Parsing

/// Parses a list of attribute specifiers.
ast_t parse_attribute_sequence(parser_t parser);

/// Parses an attributes in the form `[[...]]`
ast_t parse_attribute_specifier(parser_t parser);

#pragma mark - Declaration Parsing

/// Parses a single declaration which may contain many declarators.
/// - Parameter isDefinitionAllowed: Whether to allow function definitions.
ast_t parse_declaration(parser_t parser, bool isDefinitionAllowed);

ast_t parse_initializer(parser_t parser);

ast_t parse_declarator(parser_t parser, struct type type);

ast_t parse_function_declaration(parser_t parser, struct type type);

#pragma mark - Expression Parsing

typedef ast_t (*expression_callback)(parser_t parser, bool required);

/// Parses an expression, including the comma expression.
///
/// If this is within a comma-delimited list, use ``libkcc/parse_assignment_expression`` instead.
ast_t parse_expression(parser_t parser);

/// Parses an expression, excludes assignment and comma operations.
ast_t parse_constant_expression(parser_t parser);

ast_t parse_cast_expression(parser_t parser, bool required);

ast_t parse_multiplicative_expression(parser_t parser, bool required);

ast_t parse_additive_expression(parser_t parser, bool required);

ast_t parse_shift_expression(parser_t parser, bool required);

ast_t parse_relational_expression(parser_t parser, bool required);

ast_t parse_equality_expression(parser_t parser, bool required);

ast_t parse_bitwise_and_expression(parser_t parser, bool required);

ast_t parse_bitwise_xor_expression(parser_t parser, bool required);

ast_t parse_bitwise_or_expression(parser_t parser, bool required);

ast_t parse_logical_and_expression(parser_t parser, bool required);

ast_t parse_logical_or_expression(parser_t parser, bool required);

ast_t parse_conditional_expression(parser_t parser, bool required);

ast_t parse_assignment_expression(parser_t parser, bool required);

enum ast_kind assignment_operation(enum token_kind token);

ast_t parse_comma_operation(parser_t parser, ast_t operand);

#pragma mark - Literal Parsing

ast_t parse_string_literal(parser_t parser);

#pragma mark - Statement Parsing

/// Parse one or more statements.
ast_t parse_statements(parser_t parser);

ast_t parse_statement(parser_t parser);

ast_t parse_compound_statement(parser_t parser);

ast_t parse_if_statement(parser_t parser);

ast_t parse_print_statement(parser_t parser);

/// Parse the declaration of a variable.
ast_t parse_var_declaration(parser_t parser);

ast_t parse_assignment_statement(parser_t parser);

//MARK: - External Parsing

ast_t parse_external_declaration(parser_t parser);

#pragma mark - Utility Functions

/// Ensure that the current token is `t`, and fetch the next token. Otherwise throw an error.
void match(enum token_kind t, char *what);

/// Match a semicon and fetch the next token.
void semi();

/// Match a `{` and fetch the next token.
void lbrace();

/// Match a `}` and fetch the next token.
void rbrace();

/// Match a `[` and fetch the next token.
void lbracket();

/// Match a `]` and fetch the next token.
void rbracket();

/// Match a `(` and fetch the next token.
void lparen();

/// Match a `)` and fetch the next token.
void rparen();

/// Match a `,` and fetch the next token.
void comma();

/// Match a `:` and fetch the next token.
void colon();

/// Match an identifier and fetch the next token.
void identifier();
