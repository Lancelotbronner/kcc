//
//  parser.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "parser.h"
#include "globals.h"

#include <kcc/diagnostics.h>
#include <kcc/lexer.h>

struct ast_node *parser_parse() {
	lexer_advance();
	return parse_unit();
}

void match(enum token_kind t, char *what) {
	if (Token.kind != t)
		fatals("expected", what);
	lexer_advance();
}

void semi() {
	match(T_SEMICOLON, ";");
}

void identifier() {
	match(T_IDENTIFIER, "identifier");
}

void lparen() {
	match(T_LPAREN, "(");
}

void rparen() {
	match(T_RPAREN, ")");
}

void lbrace() {
	match(T_LCURLY, "{");
}

void rbrace() {
	match(T_RCURLY, "}");
}

void lbracket() {
	match(T_LBRACKET, "[");
}

void rbracket() {
	match(T_RBRACKET, "]");
}

void comma() {
	match(T_COMMA, ",");
}

void colon() {
	match(T_COLON, ":");
}
