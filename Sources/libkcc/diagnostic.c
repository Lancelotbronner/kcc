//
//  diagnostics.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include <kcc/diagnostic.h>
#include <kcc/diagnostics.h>

#include <kcc/print.h>
#include "diagnostics.h"
#include "globals.h"
#include "lexer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct diagnostic Diagnostic = {};

static struct diagnostic_span *_diagnostic_label() {
	int i = 0;
	while (i < DIAGNOSTIC_LABELS && Diagnostic.secondary[i].size > 0)
		i++;
	if (i < DIAGNOSTIC_LABELS)
		return nullptr;
	return &Diagnostic.secondary[i];
}

static struct diagnostic_fix *_diagnostic_fix() {
	static enum diagnostic_fix_flags FLAGS_MASK = DIAGNOSTIC_INSERT | DIAGNOSTIC_REMOVE;
	int i = 0;
	while (i < DIAGNOSTIC_FIXES && (Diagnostic.fix[i].flags ^ FLAGS_MASK) == 0)
		i++;
	if (i < DIAGNOSTIC_FIXES)
		return nullptr;
	return &Diagnostic.fix[i];
}

void diagnostic_level(enum diagnostic_level level) {
	Diagnostic.level = level;
#if DEBUG
	printf("[DIAG] severity %i\n", level);
#endif
}

void diagnostic_location(char const *source, uint32_t line, uint32_t column) {
	Diagnostic.source = source;
	Diagnostic.line = line;
	Diagnostic.column = column;
#if DEBUG
	printf("[DIAG] %i:%i @%p\n", line, column, source);
#endif
}

void diagnostic_code(char group[4], uint32_t identifier) {
	memcpy(Diagnostic.code.group, group, sizeof(char) * 4);
	Diagnostic.code.identifier = identifier;
#if DEBUG
	printf("[DIAG] %s%i\n", group, identifier);
#endif
}

void diagnostic_snippet(uint16_t location, uint16_t size, char const *label) {
	Diagnostic.primary.location = location;
	Diagnostic.primary.size = size;
	Diagnostic.primary.label = label;
#if DEBUG
	printf("[DIAG] %i:%i %s\n%.*s\n", location, size, label, size, Diagnostic.source + location);
#endif
}

void diagnostic_label(uint16_t location, uint16_t size, char const *label) {
	struct diagnostic_span *span = _diagnostic_label();
	assert(span);
	span->location = location;
	span->size = size;
	span->label = label;
}

void diagnostic_remove(uint16_t location, uint16_t count) {
	struct diagnostic_fix *fix = _diagnostic_fix();
	assert(fix);
	fix->removal.location = location;
	fix->removal.count = count;
	fix->remove = true;
}

void diagnostic_insert(uint16_t location, char const *text) {
	struct diagnostic_fix *fix = _diagnostic_fix();
	assert(fix);
	fix->insertion.location = location;
	fix->insertion.text = text;
	fix->insert = true;
}

void diagnostic_replace(uint16_t location, uint16_t count, char const *text) {
	struct diagnostic_fix *fix = _diagnostic_fix();
	assert(fix);
	fix->removal.location = location;
	fix->removal.count = count;
	fix->remove = true;
	fix->insertion.location = location;
	fix->insertion.text = text;
	fix->insert = true;
}

void diagnostic_attach() {
	Diagnostic.attached = true;
}

void diagnostic_emit() {
	diagnostic_consume(&Diagnostic);
	Diagnostic = (struct diagnostic){};
#if DEBUG
	printf("[DIAG] emit\n");
#endif
}

void fatal(char *message) {
	fprintf(stderr, "[:%zu:%zu] %s\n", Line, Column, message);
	assert(false);
	exit(EXIT_FAILURE);
}

void fatals(char *message, char *value) {
	fprintf(stderr, "[:%zu:%zu] %s: %s\n", Line, Column, message, value);
	assert(false);
	exit(EXIT_FAILURE);
}

void fatald(char *message, int value) {
	fprintf(stderr, "[:%zu:%zu] %s: %d\n", Line, Column, message, value);
	assert(false);
	exit(EXIT_FAILURE);
}

void fatalc(char *message, int value) {
	fprintf(stderr, "[:%zu:%zu] %s: %c\n", Line, Column, message, value);
	assert(false);
	exit(EXIT_FAILURE);
}

void fatalt(char *s) {
	fatals(s, token_nameof(Token.kind));
}
