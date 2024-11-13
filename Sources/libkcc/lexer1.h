//
//  scanner.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <kcc/lexer1.h>

#include "globals.h"

/// The current character.
int Character;

/// Characters put back by scanner.
char Putback[8];

/// The index of the current Putback character.
int PutbackIndex;

/// Return the position of character c in string s, or -1 if c not found.
size_t chrpos(char *s, int c);

/// Get the next character from the input file.
int next();

/// Put back an unwanted character.
void putback(int c);

/// Skip past input that we don't need to deal with, i.e. whitespace, newlines.
void skip();

/// Scan and return an integer literal value from the input file.
size_t scan_int(int c);

/// Scan an identifier from the input file and store it in `buf[]`. Return the identifier's length.
int scan_identifier(int c, char *buf, int lim);

/// Given a character, return the matching token or ``T_UNKNOWN`` if none matches.
enum token_kind scan_fixed_len(int c);

/// Scan a multi-character token, returns whether token is valid.
bool scan_varlen(int c);

/// Given a word from the input, return the matching keyword token number or 0 if it's not a keyword.
enum token_kind keyword(char *s);
