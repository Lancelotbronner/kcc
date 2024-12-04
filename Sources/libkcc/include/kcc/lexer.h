//
//  lexer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#pragma once

#include <kcc/token.h>
#include <kcc/types.h>

#include <stdbool.h>
#include <stddef.h>

//TODO: Move global lexer state into struct

scanner_t Scanner;
lexer_t Lexer;

enum lexer_mode : unsigned char {
	LEXER_SOURCE,
	LEXER_DIRECTIVE,
	LEXER_ATTRIBUTES,
};

/// The token that was just parsed.
struct token Token;

/// The size of the current token.
ptrdiff_t TokenLength;

/// Points to the current token within the source buffer.
char const *TokenSource;

/// Returns the size of the `lexer` struct.
size_t lexer_size();

/// Allocates a lexer.
lexer_t lexer_alloc();

/// Initializes a new lexer reading from the specified scanner.
void lexer_init(lexer_t lexer, scanner_t scanner);

/// Returns the lexer's current mode.
/// - SeeAlso: ``lexer_enter``
enum lexer_mode lexer_mode(lexer_t lexer);

/// Makes the lexer enter the specified mode.
/// - Parameter mode: The mode to enter.
/// - SeeAlso: ``lexer_mode``
void lexer_enter(lexer_t lexer, enum lexer_mode mode);

/// Returns whether the lexer is currently at end of file.
bool lexer_eof(lexer_t lexer);

/// Advance the lexer to the next token.
///
/// Returns `true` if token valid, `false` if no tokens left.
bool lexer_advance(lexer_t lexer);

/// Consumes the token if it matches, returns whether it matched.
/// - Parameter token: The token to match.
bool lexer_match(lexer_t lexer, enum token_kind token);

/// Current integer literal, if any.
size_t IntegerLiteral;

/// Current string literal, if any.
const char *StringLiteral;

/// Current length of a string or character literal, if any.
size_t LiteralLength;
