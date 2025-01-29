//
//  ast.h
//  kcc
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "ast_storage.h"
#include "token_modifiers.h"
#include "type.h"
#include <kcc/types.h>
#include "symbol.h"

#include <stddef.h>

#pragma mark - Specialized Nodes

struct ast_unit {
	struct ast_storage declarations;
};

struct ast_integer_literal {
	/// The value of this integer literal.
	size_t value;
	struct token_integer_modifier modifier;
};

struct ast_unary_expression {
	/// The resulting type of this binary expression, if any.
	struct type type;
	/// The value tree.
	ast_t operand;
};

struct ast_binary_expression {
	/// The resulting type of this binary expression, if any.
	struct type type;
	/// Left value tree.
	ast_t lhs;
	/// Right value tree.
	ast_t rhs;
};

struct ast_identifier {
	/// The associated symbol.
	struct symbol *symbol;
};

struct ast_compound {
	/// The contents of this compound.
	struct ast_storage contents;
};

struct ast_if {
	/// The condition which determines the branch to take.
	ast_t condition;
	/// The contents of the `true` branch.
	ast_t then_tree;
	/// The contents of the `false` branch, optional.
	ast_t else_tree;
};

struct ast_while {
	/// The condition for the loop to continue executing.
	ast_t condition;
	/// The contents of the loop.
	ast_t loop;
};

struct ast_for {
	/// The convenience declaration, optional.
	ast_t declaration;
	/// The operation to make before the loop starts, optional.
	ast_t preop;
	/// The condition for the loop to continue executing.
	ast_t condition;
	/// The operation to make after every loop execution.
	ast_t postop;
	/// The contents of the loop.
	ast_t loop;
};

struct ast_function {
	//TODO: return type
	/// The associated symbol of the function.
	struct symbol *symbol;
	/// The contents of the function's declaration, optional.
	ast_t declaration;
	/// The contents of the function's definition, optional.
	ast_t definition;
};

struct ast_call_expression {
	/// The expression being called.
	ast_t operand;
	//TODO: Use ast_storage for more than one arguments
	/// The arguments of the function, if any.
	struct ast_storage arguments;
};

struct ast_declaration {
	/// The base type of this declaration.
	struct type type;
	ast_t type_tree;
	/// The declarators of this declaration.
	struct ast_storage declarators;
};

struct ast_declarator {
	/// The type of this declarator.
	struct type type;
	/// The symbol of this declarator, if any.
	struct symbol *symbol;
	/// The inner declarator of this declarator.
	ast_t declarator;
	/// The initialization of this declarator, if any..
	ast_t initializer;
};

struct ast_pointer_declarator {
	/// The inner declarator of this declarator.
	ast_t declarator;
	//TODO: attribute list
	//TODO: type qualifiers
};

struct ast_group_declarator {
	/// The inner declarator of this declarator.
	ast_t declarator;
};

struct ast_identifier_declarator {
	/// The symbol associated with this declarator.
	struct symbol *symbol;
	//TODO: attribute list
};

struct ast_array_declarator {
	/// The inner declarator of this declarator.
	ast_t declarator;
	/// The expression of the array's size.
	ast_t size_tree;
	//TODO: type qualifiers
	//TODO: static flag
	//TODO: variable length flag
};

struct ast_function_declarator {
	/// The inner declarator of this declarator.
	ast_t declarator;
	//TODO: attribute list
};

struct ast_generic_expression {
	/// The expression whose type controls the generic selection.
	ast_t control;
	/// The generic associations.
	struct ast_storage associations;
};

struct ast_generic_association {
	/// The type expression or nil if its a default clause.
	ast_t key;
	/// The expression associated with the type.
	ast_t value;
};

struct ast_parenthesized_expression {
	/// The contents of the parenthesis.
	ast_t expression;
};

struct ast_comma_expression {
	ast_t discarded;
	ast_t returned;
};

struct ast_cast_expression {
	struct type type;
	ast_t operand;
	ast_t type_tree;
};

struct ast_string_literal {
	char *value;
	enum token_encoding_modifier encoding : 3;
};

#pragma mark - Generic Node

/// AST node kinds.
enum ast_kind : unsigned char {
	N_UNKNOWN,

	// Translation Unit
	N_UNIT,

	// Declarations
	N_DECLARATION,
	N_FUNCTION,
	N_DECLARATOR,
	N_POINTER_DECLARATOR,
	N_GROUP_DECLARATOR,
	N_IDENTIFIER_DECLARATOR,
	N_ARRAY_DECLARATOR,
	N_FUNCTION_DECLARATOR,

	// Statements
	N_STATEMENTS,
	N_COMPOUND,
	N_IF,
	N_WHILE,
	N_FOR,

	// Primary Expressions
	N_INTEGER_LITERAL,
	N_IDENTIFIER,
	N_PARENTHESIZED_EXPRESSION,
	N_GENERIC,
	N_GENERIC_ASSOCIATION,

	// Postfix Expressions
	N_SUBSCRIPT,
	N_CALL,
	N_ACCESS,
	N_INDIRECT_ACCESS,
	N_INCREMENT_POSTFIX,
	N_DECREMENT_POSTFIX,
	N_COMPOUND_LITERAL,

	// Prefix Expressions
	N_INCREMENT_PREFIX,
	N_DECREMENT_PREFIX,
	N_ADDRESSOF,
	N_DEREFERENCE,
	N_POSITIVE,
	N_NEGATIVE,
	N_BITWISE_NOT,
	N_LOGICAL_NOT,
	N_SIZEOF_TYPE,
	N_SIZEOF_EXPRESSION,
	N_ALIGNOF,

	// Cast Expressions
	N_CAST,

	// Multiplicative Expressions
	N_MULTIPLY,
	N_DIVIDE,
	N_MODULO,

	// Additive Expressions
	N_ADD,
	N_SUBTRACT,

	// Bitwise Shift Expressions
	N_LSHIFT,
	N_RSHIFT,

	// Relational Expressions
	N_GT,
	N_LT,
	N_LTE,
	N_GTE,

	// Equality Expressions
	N_EQ,
	N_NEQ,

	// Bitwise Expressions
	N_BITWISE_AND,
	N_BITWISE_XOR,
	N_BITWISE_OR,

	// Logical Expressions
	N_LOGICAL_AND,
	N_LOGICAL_OR,

	// Conditional Expression
	N_CONDITIONAL,

	// Assignment Expressions
	N_ASSIGN,
	N_ASSIGN_MULTIPLY,
	N_ASSIGN_DIVIDE,
	N_ASSIGN_MODULO,
	N_ASSIGN_ADD,
	N_ASSIGN_SUBTRACT,
	N_ASSIGN_LSHIFT,
	N_ASSIGN_RSHIFT,
	N_ASSIGN_BITWISE_AND,
	N_ASSIGN_BITWISE_XOR,
	N_ASSIGN_BITWISE_OR,

	// Comma Expression
	N_COMMA,

	// Structural Components
	N_LVIDENT,

	AST_MAX,
};

/// Abstract Syntax Tree structure
struct ast_node {
	/// "Operation" to be performed on this tree.
	enum ast_kind op;
	union {
		struct ast_unit unit;
		struct ast_integer_literal integer_literal;
		struct ast_unary_expression unary_expression;
		struct ast_binary_expression binary_expression;
		struct ast_identifier identifier;
		struct ast_compound compound;
		struct ast_if if_statement;
		struct ast_while while_statement;
		struct ast_for for_statement;
		struct ast_function function_declaration;
		struct ast_call_expression call_expression;
		struct ast_declaration declaration;
		struct ast_declarator declarator;
		struct ast_pointer_declarator pointer_declarator;
		struct ast_group_declarator group_declarator;
		struct ast_identifier_declarator identifier_declarator;
		struct ast_array_declarator array_declarator;
		struct ast_function_declarator function_declarator;
		struct ast_generic_expression generic_expression;
		struct ast_generic_association generic_association;
		struct ast_parenthesized_expression parenthesized_expression;
		struct ast_comma_expression comma_expression;
		struct ast_cast_expression cast_expression;
	};
};
