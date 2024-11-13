//
//  scanner2.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-10-24.
//

#pragma once

#include <stdint.h>

typedef struct scanner *scanner_t;

size_t scanner_size();
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

/// Moves the anchor to the current buffer location, returns the number of bytes jumped.
/// - Returns: The length of the range formed by the previous and new anchor location.
ptrdiff_t scanner_anchor(scanner_t scanner);

/// Puts a character back into the scanner's stream.
/// - Parameters:
///   - scanner: The scanner to put into.
///   - character: The character to put back.
void scanner_putback(scanner_t scanner, char character);

/// Returns whether the scanner has at least `count` additional characters.
/// - Parameter scanner: The scanner to query.
bool scanner_atleast(scanner_t scanner, int count);

/// Returns a character ahead in the stream, doesn't check bounds.
/// - Parameters:
///   - scanner: The scanner to query.
///   - offset: How far ahead to look, 0 means the next character.
char scanner_lookahead(scanner_t scanner, int offset);
