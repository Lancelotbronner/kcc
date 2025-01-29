//
//  parser_utils.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-12-08.
//

#include "parser.h"
#include "globals.h"

#include <kcc/diagnostics.h>
#include <kcc/lexer.h>

void match(enum token_kind t, char *what) {
	if (Token.kind != t)
		fatals("expected", what);
	lexer_advance(Lexer);
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

