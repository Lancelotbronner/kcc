//
//  ast.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "ast_node.h"

/// Allocates a new AST node;
struct ast_node *ast_alloc();

#pragma mark - Node Constructors

/// Initializes a binary AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - op: The binary operation.
///   - left: The tree on the left-hand side of the operator.
///   - right: The tree on the right-hand side of the operator.
void ast_binary_expression(struct ast_node *node, enum ast_kind op, struct ast_node *left, struct ast_node *right);

/// Initializes a unary AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - op: The binary operation.
///   - operand: The tree of the operand.
void ast_unary_expression(struct ast_node *node, enum ast_kind op, struct ast_node *operand);

/// Initializes an integer literal AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - value: The value of the integer literal.
///   - base: The base of the integer literal, or `BASE_NONE`.
///   - suffix: The suffix of the integer literal, or `IMOD_NONE`.
void ast_integer_literal(struct ast_node *node, size_t value, enum token_base_modifier base, enum token_integer_modifier suffix);

/// Initializes an identifier AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - symbol: The symbol associated to the identifier.
void ast_identifier(struct ast_node *node, struct symbol *symbol);

/// Initializes a compound AST node.
/// - Parameter node: The node to initialize.
void ast_compound(struct ast_node *node);

/// Initializes an `if` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - condition: The condition of the `if` statement.
///   - then_tree: The tree when `condition` is `true`.
///   - else_tree: The tree when `condition` is `false`.
void ast_if(struct ast_node *node, struct ast_node *condition, struct ast_node *then_tree, struct ast_node *else_tree);

/// Initializes a `while` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - condition: The condition of the `while` statement.
///   - loop: The contents of the loop.
void ast_while(struct ast_node *node, struct ast_node *condition, struct ast_node *loop);

/// Initializes a `for` statement AST node.
/// - Parameters:
///   - node: The node to initialize.
///   - declaration: The convenience declaration, optional.
///   - preop: The operation to make before the loop starts, optional.
///   - condition: The condition for the loop to continue executing.
///   - postop: The operation to make after every loop execution.
///   - loop: The contents of the loop.
void ast_for(struct ast_node *node, struct ast_node *declaration, struct ast_node *preop, struct ast_node *condition, struct ast_node *postop, struct ast_node *loop);

void ast_function(struct ast_node *node, struct symbol *symbol, struct ast_node *definition);

void ast_call(struct ast_node *node, struct ast_node *function, struct ast_storage arguments);

void ast_identifier_declarator(struct ast_node *node, struct type type, struct symbol *symbol);

void ast_function_declarator(struct ast_node *node, struct ast_node *declaration/* TODO: parameters */);

void ast_pointer_declarator(struct ast_node *node, struct ast_node *declarator);
void ast_group_declarator(struct ast_node *node, struct ast_node *declarator);

#pragma mark - Node Properties

/// Provides the type of the node, if any.
/// - Parameter node: The node whose type to extract.
struct type ast_type(struct ast_node *node);
