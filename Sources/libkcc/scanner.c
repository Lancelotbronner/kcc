//
//  scanner2.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-10-24.
//

#include <kcc/scanner.h>

#include <kcc/diagnostic.h>

#include "globals.h" // Line, Column

#include <assert.h>
#include <ctype.h>
#include <stdlib.h> // malloc

//FIXME: Workaround for lexer
struct scanner __Scanner;
scanner_t Scanner = &__Scanner;

#define PUTBACK_MAX 2

struct scanner {
	const char *buffer;
	const char *eof;

	/// Points to the start of the current token.
	const char *anchor;
	/// Points to the current reading head.
	const char *head;

	/// Character currently pointed to by the scanner.
	char character;

	char putback[PUTBACK_MAX];
	unsigned putback_index;
};

size_t scanner_size() {
	return sizeof(struct scanner);
}

scanner_t scanner_alloc() {
	return malloc(scanner_size());
}

void scanner_init(scanner_t scanner, char const *buffer, size_t size) {
	assert(buffer);
	assert(size > 0);
	*scanner = (struct scanner) {
		.buffer = buffer,
		.eof = buffer + size,

		.anchor = buffer,
		.head = buffer + 1,

		.character = *buffer,
		.putback_index = 0,
	};
}

char scanner_peek(scanner_t scanner) {
	return scanner->character;
}

bool scanner_eof(scanner_t scanner) {
	return scanner->head > scanner->eof;
}

static inline void scanner_handle_line_continuation(scanner_t scanner) {
	// Whitespace after a backslash is a potential line continuation.
	if (scanner->character != '\\' || scanner_eof(scanner) || !isspace(scanner_lookahead(scanner, 0)))
		return;

	// Handle whitespace between the backslash and the newline to help diagnose errors.
	char const *head = scanner->head;
	while (*head++ != '\n' && head != scanner->eof && isspace(*head))
		continue;

	// Diagnose whitespace between the backslash and newline.
	ptrdiff_t length = head - scanner->head - 1;
	if (length > 0 && !(length == 1 && head < scanner->eof && *head == '\r')) {
		diagnostic_code("LEX", 2);
		diagnostic_level(DIAGNOSTIC_ERROR);
		diagnostic_location(scanner->head, (uint32_t)Line, (uint32_t)Column);
		diagnostic_snippet(0, length, "invalid whitespace in line continuation");
		diagnostic_emit();
	}

	scanner->head = head;
	scanner->character = *head;
	//TODO: Update line & column numbers
}

/// Advances to the next character, taking newline breaks into account.
///
/// - See Also: `5.1.1.2 Translation phases`
bool scanner_advance(scanner_t scanner) {
	// Take putback in account.
	if (scanner->putback_index) {
		scanner->character = scanner->putback[--scanner->putback_index];
		return true;
	}

	/// Check whether we have more to read.
	if (scanner_eof(scanner)) {
		scanner->character = 0;
		return false;
	}

	// Advance to the next character.
	scanner->character = *scanner->head++;

	//TODO: Keep track of line and column in scanner
//	// Keep our physical line number in sync with the file.
//	if (*CurPtr == '\n') {
//		Line++;
//		Column = 1;
//	}

	// Handle line continuations if required.
	scanner_handle_line_continuation(scanner);
}

void scanner_putback(scanner_t scanner, char c) {
	if (scanner->putback_index == PUTBACK_MAX)
		fatal("Putback stack overflow");
	scanner->putback[scanner->putback_index++] = c;
}

bool scanner_consume(scanner_t scanner, char character) {
	bool match = scanner->character == character;
	if (match)
		scanner_advance(scanner);
	return match;
}

bool scanner_atleast(scanner_t scanner, int count) {
	return scanner->head + count - 1 < scanner->eof;
}

char scanner_lookahead(scanner_t scanner, int offset) {
	assert(scanner->head + offset < scanner->eof);
	return *(scanner->head + offset);
}

void scanner_anchor(scanner_t scanner) {
	scanner->anchor = scanner->head - 1;
}

ptrdiff_t scanner_length(scanner_t scanner) {
	return scanner->head - scanner->anchor - 1;
}

char const *scanner_token(scanner_t scanner) {
	return scanner->anchor;
}
