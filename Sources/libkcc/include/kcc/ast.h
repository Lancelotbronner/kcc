//
//  ast.h
//  kcc
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <kcc/ast_node.h>

/// Allocates a new AST node;
ast_t ast_alloc();

#pragma mark - Node Constructors

void ast_unit(ast_t node, struct ast_storage declarations);

/// Initializes a binary AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - op: The binary operation.
///   - left: The tree on the left-hand side of the operator.
///   - right: The tree on the right-hand side of the operator.
void ast_binary_expression(ast_t node, enum ast_kind op, ast_t left, ast_t right);

/// Initializes a unary AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - op: The binary operation.
///   - operand: The tree of the operand.
void ast_unary_expression(ast_t node, enum ast_kind op, ast_t operand);

/// Initializes an integer literal AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - value: The value of the integer literal.
///   - modifier: The modifier of the integer literal.
void ast_integer_literal(ast_t node, size_t value, struct token_integer_modifier modifier);

/// Initializes an identifier AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - symbol: The symbol associated to the identifier.
void ast_identifier(ast_t node, struct symbol *symbol);

/// Initializes a compound AST node.
/// - Parameter node: The node to initialize.
void ast_compound(ast_t node);

/// Initializes an `if` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - condition: The condition of the `if` statement.
///   - then_tree: The tree when `condition` is `true`.
///   - else_tree: The tree when `condition` is `false`.
void ast_if(ast_t node, ast_t condition, ast_t then_tree, ast_t else_tree);

/// Initializes a `while` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - condition: The condition of the `while` statement.
///   - loop: The contents of the loop.
void ast_while(ast_t node, ast_t condition, ast_t loop);

/// Initializes a `for` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - declaration: The convenience declaration, optional.
///   - preop: The operation to make before the loop starts, optional.
///   - condition: The condition for the loop to continue executing.
///   - postop: The operation to make after every loop execution.
///   - loop: The contents of the loop.
void ast_for(ast_t node, ast_t declaration, ast_t preop, ast_t condition, ast_t postop, ast_t loop);

void ast_function(ast_t node, struct symbol *symbol, ast_t declaration, ast_t definition);

void ast_call(ast_t node, ast_t function, struct ast_storage arguments);

void ast_declaration(ast_t node, struct type type, struct ast_storage declarators);

void ast_declarator(ast_t node, struct symbol *symbol, struct type type, ast_t declarator, ast_t initializer);
void ast_pointer_declarator(ast_t node, ast_t declarator);
void ast_group_declarator(ast_t node, ast_t declarator);
void ast_identifier_declarator(ast_t node, struct type type, struct symbol *symbol);
void ast_array_declarator(ast_t node, ast_t declarator, ast_t size);
void ast_function_declarator(ast_t node, /* TODO: parameters , */ ast_t declarator);

#pragma mark - Node Properties

/// Provides the type of the node, if any.
/// - Parameter node: The node whose type to extract.
struct type ast_type(ast_t node);
