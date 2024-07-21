//
//  scanner.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "token.h"

#include <stdbool.h>
#include <stddef.h>

/// Length of symbols in input
#define TEXTLEN 512

/// Last token scanned.
struct token Token;

/// Last integer literal scanned.
size_t Integer;

/// Last identifier or string scanned.
char Text[TEXTLEN + 1];

/// Scan and return the next token found in the input.
///
/// Return `true` if token valid, `false` if no tokens left.
bool scan();
