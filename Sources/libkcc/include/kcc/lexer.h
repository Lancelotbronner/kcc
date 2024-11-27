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

/// Advance the lexer to the next token.
///
/// Returns `true` if token valid, `false` if no tokens left.
bool lexer_advance();

/// Returns whether the lexer is currently at end of file.
bool lexer_eof();

/// Current integer literal, if any.
size_t IntegerLiteral;

/// Current string literal, if any.
const char *StringLiteral;

/// Current length of a string or character literal, if any.
size_t LiteralLength;
