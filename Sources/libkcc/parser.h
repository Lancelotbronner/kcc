//
//  parser.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <kcc/parser.h>

#include <kcc/ast_node.h>
#include <kcc/token_kind.h>

struct type Type;
struct symbol *Symbol;

struct ast_node *parse_unit();

#pragma mark - Attribute Parsing

struct ast_node *parse_attribute_sequence();

#pragma mark - Declaration Parsing

struct ast_node *parse_declaration();

struct ast_node *parse_initializer();

struct ast_node *parse_declarator(struct type type);

struct ast_node *parse_function_declaration(struct type type);

#pragma mark - Expression Parsing

typedef struct ast_node *(*expression_callback)(bool required);

/// Parses an expression, including the comma expression.
///
/// If this is within a comma-delimited list, use ``libkcc/parse_assignment_expression`` instead.
struct ast_node *parse_expression();

/// Parses an expression, excludes assignment and comma operations.
struct ast_node *parse_constant_expression();

struct ast_node *parse_cast_expression(bool required);

struct ast_node *parse_multiplicative_expression(bool required);

struct ast_node *parse_additive_expression(bool required);

struct ast_node *parse_shift_expression(bool required);

struct ast_node *parse_relational_expression(bool required);

struct ast_node *parse_equality_expression(bool required);

struct ast_node *parse_bitwise_and_expression(bool required);

struct ast_node *parse_bitwise_xor_expression(bool required);

struct ast_node *parse_bitwise_or_expression(bool required);

struct ast_node *parse_logical_and_expression(bool required);

struct ast_node *parse_logical_or_expression(bool required);

struct ast_node *parse_conditional_expression(bool required);

struct ast_node *parse_assignment_expression(bool required);

enum ast_kind assignment_operation(enum token_kind token);

struct ast_node *parse_comma_operation(struct ast_node *operand);

#pragma mark - Literal Parsing

struct ast_node *parse_string_literal();

#pragma mark - Statement Parsing

/// Parse one or more statements.
struct ast_node *parse_statements();

struct ast_node *parse_statement();

struct ast_node *parse_compound_statement();

struct ast_node *parse_if_statement();

struct ast_node *parse_print_statement();

/// Parse the declaration of a variable.
struct ast_node *parse_var_declaration();

struct ast_node *parse_assignment_statement();

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
