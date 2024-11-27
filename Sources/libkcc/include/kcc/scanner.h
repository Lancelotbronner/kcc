//
//  scanner2.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-10-24.
//

#pragma once

#include <kcc/types.h>

#include <stdint.h>

/// Returns the size of the `scanner` struct.
size_t scanner_size();

/// Allocates a scanner.
scanner_t scanner_alloc();

/// Initializes a new scanner reading from the specified buffer.
/// - Parameters:
///   - scanner: The scanner to initialize.
///   - buffer: The buffer to read from.
///   - size: The size of the buffer.
void scanner_init(scanner_t scanner, char const *buffer, size_t size);

/// Returns the current character.
/// - Parameter scanner: The scanner to peek from.
char scanner_peek(scanner_t scanner);

/// Returns whether the scanner has reached end of file.
/// - Parameter scanner: The scanner to query.
bool scanner_eof(scanner_t scanner);

/// Advances to the next character, taking newline breaks into account.
/// - SeeAlso: 5.1.1.2 Translation phases
/// - Returns: Whether the scanner has advanced.
bool scanner_advance(scanner_t scanner);

/// Advances the scanner if the current character matches the provided one.
/// - Parameters:
///   - scanner: The scanner to advance.
///   - character: The character to match.
/// - Returns: Whether the characters matched.
bool scanner_consume(scanner_t scanner, char character);

/// Puts a character back into the scanner's stream.
/// - Parameter character: The character to put back.
void scanner_putback(scanner_t scanner, char character);

/// Returns whether the scanner has at least `count` additional characters.
/// - Parameter count: The minimum remaining characters required to return `true`.
bool scanner_atleast(scanner_t scanner, int count);

/// Returns a character ahead in the stream, doesn't check bounds.
/// - Parameter offset: How far ahead to look, 0 means the next character.
char scanner_lookahead(scanner_t scanner, int offset);

/// Moves the anchor to the current buffer location, returns the number of bytes jumped.
/// - Returns: The length of the formed token.
void scanner_anchor(scanner_t scanner);

/// Returns the length of the scanner's current token.
ptrdiff_t scanner_length(scanner_t scanner);

/// Returns a pointer to the start of the current token within the source buffer.
char const *scanner_token(scanner_t scanner);
