//
//  scanner.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include "scanner.h"

#include <kcc/diagnostics.h>

#include <ctype.h>
#include <string.h>

bool Preprocessor = false;

bool scan() {
	// skip whitespace
	skip();

	// try determining the token based on the input character
	Token.kind = scan_fixed_len(Character);

	// if its not a single character token, scan multi-character tokens
	if (Token.kind == T_UNKNOWN && !scan_varlen(Character))
		return false;

	// we found a token
	return true;
}

enum token_kind scan_fixed_len(int c) {
	switch (c) {
	case EOF: return T_EOF;
	case '\0': return T_EOF;
	case '(': return T_LPAREN;
	case ')': return T_RPAREN;
	case '[': return T_LBRACKET;
	case ']': return T_RBRACKET;
	case '{': return T_LCURLY;
	case '}': return T_RCURLY;
	case '*': return T_ASTERISK;
	case '/': return T_DIVIDE;
	case ',': return T_COMMA;
	case ';': return T_SEMICOLON;
	case '.': return T_ACCESS;
	case '^': return T_CARET;

	case '#':
		Preprocessor = true;
		return T_DIRECTIVE;
	case '+':
		switch (next()) {
		case '+': return T_INCREMENT;
		default:
			putback(Character);
			return T_ADD;
		}
	case '-':
		switch (next()) {
		case '>': return T_INDIRECT_ACCESS;
		case '-': return T_DECREMENT;
		default:
			putback(Character);
			return T_SUBTRACT;
		}
	case '|':
		switch (next()) {
		case '|': return T_OR;
		default:
			putback(Character);
			return T_PIPE;
		}
	case '=':
		if (next() == '=')
			return T_EQ;
		putback(Character);
		return T_ASSIGN;

	case '&':
		if (next() == '&')
			return T_AND;
		putback(Character);
		return T_AMPERSAND;

	case '!':
		switch (next()) {
		case '=': return T_NEQ;
		default:
			putback(Character);
			return T_EXCLAIM;
		}
	case '<':
		switch (next()) {
		case '=': return T_LTE;
		case '<': return T_LSHIFT;
		default:
			putback(Character);
			return T_LT;
		}
	  case '>':
		switch (next()) {
		case '=': return T_GTE;
		case '>': return T_RSHIFT;
		default:
			putback(Character);
			return T_GT;
		}
	default:
		return T_UNKNOWN;
	}
}

bool scan_varlen(int c) {
	if (isdigit(c)) {
		Integer = scan_int(c);
		Token.kind = T_INTEGER_LITERAL;
		return true;
	}

	if (isalpha(c) || '_' == c) {
		// read in a keyword or identifier
		int length = scan_identifier(c, Text, TEXTLEN);
		if (!length)
			return false;

		// if it's a recognised keyword, return that token
		Token.kind = keyword(Text);

		// not a recognised keyword, so it must be an identifier
		if (Token.kind == T_UNKNOWN)
			Token.kind = T_IDENTIFIER;

		return true;
	}

	// the character isn't part of any recognised token, error
	fatalc("unrecognized character", c);
	return false;
}

#pragma mark - Scan Functions

size_t scan_int(int c) {
	ssize_t k, val = 0;

	// convert each character into an int value
	while ((k = chrpos("0123456789", c)) >= 0 && k < 10) {
		val = val * 10 + k;
		c = next();
	}

	// we hit a non-integer character, put it back.
	putback(c);
	return val;
}

int scan_identifier(int c, char *buf, int lim) {
	int i = 0;

	// allow digits, alpha and underscores
	while (isalpha(c) || isdigit(c) || '_' == c) {
		// error if we hit the identifier length limit,
		// else append to buf[] and get next character
		if (lim - 1 == i) {
			printf("identifier too long on line %zu\n", Line);
			return 0;
		} else if (i < lim - 1) {
			buf[i++] = c;
		}
		c = next();
	}
	// we hit a non-valid character, put it back.
	// NUL-terminate the buf[] and return the length
	putback(c);
	buf[i] = '\0';
	return i;
}

enum token_kind keyword(char *s) {
#define _keyword(n, match, kind) (strcmp(s, &match[n]) ? T_UNKNOWN : kind)
#define _keyword1(match, kind) _keyword(1, match, kind)
#define _keyword2(match, kind) _keyword(2, match, kind)
	switch (*s++) {
	case 'b': return _keyword1("bool", T_BOOL);
	case 'c': return _keyword1("char", T_CHAR);
	case 'e': return _keyword1("else", T_ELSE);
	case 'l': return _keyword1("long", T_LONG);
	case 'r': return _keyword1("return", T_RETURN);
	case 'u': return _keyword1("unsigned", T_UNSIGNED);
	case 'w': return _keyword1("while", T_WHILE);

	case 'i':
		switch (*s++) {
		case 'f': return _keyword2("if", T_IF);
		case 'n': return _keyword2("int", T_INT);
		default: break;
		}
	case 's':
		switch (*s++) {
		case 'i': return _keyword2("signed", T_SIGNED);
		case 'h': return _keyword2("short", T_SHORT);
		default: break;
		}
	}
	return T_UNKNOWN;
}

#pragma mark - Utility Functions

size_t chrpos(char *s, int c) {
	char *p;
	p = strchr(s, c);
	return (p ? p - s : -1);
}

int next() {
	if (Putback) {
		Character = Putback;
		Putback = 0;
		return Character;
	}

	Character = fgetc(Infile);

	if (Character == '\n') {
		Line++;
		Column = 0;
		Preprocessor = false;
	}
	Column++;

	return Character;
}

void putback(int c) {
	Putback = c;
}

void skip() {
	next();
	while (isspace(Character))
		next();
}
