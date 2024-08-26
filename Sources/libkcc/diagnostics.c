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
#include "scanner.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct diagnostic Diagnostic = {};

static struct diagnostic_span *_diagnostics_label() {
	int i = 0;
	while (i < DIAGNOSTIC_LABELS && Diagnostic.secondary[i].size > 0)
		i++;
	if (i < DIAGNOSTIC_LABELS)
		return nullptr;
	return &Diagnostic.secondary[i];
}

static struct diagnostic_fix *_diagnostics_fix() {
	static enum diagnostic_fix_flags FLAGS_MASK = DIAGNOSTIC_INSERT | DIAGNOSTIC_REMOVE;
	int i = 0;
	while (i < DIAGNOSTIC_FIXES && (Diagnostic.fix[i].flags ^ FLAGS_MASK) == 0)
		i++;
	if (i < DIAGNOSTIC_FIXES)
		return nullptr;
	return &Diagnostic.fix[i];
}

void diagnostics_level(enum diagnostic_level level) {
	Diagnostic.level = level;
}

void diagnostics_location(char *source, uint32_t line, uint32_t column) {
	Diagnostic.source = source;
	Diagnostic.line = line;
	Diagnostic.column = column;
}

void diagnostics_code(char group[4], uint32_t identifier) {
	memcpy(Diagnostic.code.group, group, sizeof(char) * 4);
	Diagnostic.code.identifier = identifier;
}

void diagnostics_snippet(uint16_t location, uint16_t size, char *label) {
	Diagnostic.primary.location = location;
	Diagnostic.primary.size = size;
	Diagnostic.primary.label = label;
}

void diagnostics_label(uint16_t location, uint16_t size, char *label) {
	struct diagnostic_span *span = _diagnostics_label();
	assert(span);
	span->location = location;
	span->size = size;
	span->label = label;
}

void diagnostics_remove(uint16_t location, uint16_t count) {
	struct diagnostic_fix *fix = _diagnostics_fix();
	assert(fix);
	fix->removal.location = location;
	fix->removal.count = count;
	fix->remove = true;
}

void diagnostics_insert(uint16_t location, char *text) {
	struct diagnostic_fix *fix = _diagnostics_fix();
	assert(fix);
	fix->insertion.location = location;
	fix->insertion.text = text;
	fix->insert = true;
}

void diagnostics_replace(uint16_t location, uint16_t count, char *text) {
	struct diagnostic_fix *fix = _diagnostics_fix();
	assert(fix);
	fix->removal.location = location;
	fix->removal.count = count;
	fix->remove = true;
	fix->insertion.location = location;
	fix->insertion.text = text;
	fix->insert = true;
}

void diagnostics_attach() {
	Diagnostic.attached = true;
}

void diagnostics_emit() {
	diagnostic_consume(&Diagnostic);
	Diagnostic = (struct diagnostic){};
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
