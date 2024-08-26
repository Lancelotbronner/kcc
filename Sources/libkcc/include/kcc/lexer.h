//
//  lexer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#pragma once

#include "token.h"

#include <stdbool.h>
#include <stddef.h>

//TODO: Turn lexers into a virtual object to invoke by the parser?
// So we can switch lexers on the go

/// The token that was just parsed.
struct token Token;

/// The length of the current token in bytes.
size_t TokenLength;

/// Advance the lexer to the next token.
///
/// Returns `true` if token valid, `false` if no tokens left.
bool lexer_advance();

/// Length of symbols in input
#define TEXTLEN 512

/// Last integer literal scanned.
size_t IntegerLiteral;

char *StringLiteral;
