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

//TODO: Have a stack of lexer state
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

//MARK: - Lexer V2

struct lexer_state {

};

/// Push a new state onto the stack.
/// - Parameter state: The lexer state to use from now on.
void lexer_push(struct lexer_state state);

/// Pops a state from the stack.
void lexer_pop();

/// Produces the current state of the lexer.
struct lexer_state lexer_state();
