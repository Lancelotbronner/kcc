//
//  parser_type.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include "parser.h"

#include <kcc/ast.h>
#include <kcc/lexer.h>

#pragma mark - Type Specifier Parsing

static ast_t parse_bitint_specifier(parser_t parser) {
	// Consume the keyword
	lexer_advance(Lexer);

	lparen();
	ast_t size = parse_constant_expression(parser);
	rparen();

	//TODO: Modify type
	Type.kind = TYP_BITINT;

	//TODO: return node
	return 0;
}

static ast_t parse_type_component(parser_t parser) {
	switch (Token.kind) {
		// Specifier
	case T_BITINT: return parse_bitint_specifier(parser);
	case T_VOID:
	case T_CHAR:
	case T_SHORT:
	case T_LONG:
	case T_FLOAT:
	case T_DOUBLE:
	case T_SIGNED:
	case T_UNSIGNED:
	case T_BOOL:
	case T_COMPLEX:
	case T_DECIMAL32:
	case T_DECIMAL64:
	case T_DECIMAL128:
		// Storage
	case T_AUTO:
	case kconstexpr:
	case kextern:
	case kregister:
	case kstatic:
	case kthread_local:
	case ktypedef:
		// Structures
	case kstruct:
	case kunion:
		// Enums
	case kenum:
		// Atomics
	case T_ATOMIC:
		// Type Of
	case ktypeof:
	case ktypeof_unqual:
		// Default
	default: return nullptr;
	}
}

#pragma mark - Type Parsing

ast_t parse_type(parser_t parser) {
	Type = (struct type){};
	return parse_type_component(parser);
}
