//
//  diagnostics.c
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include <kcc/diagnostics.h>

#include <kcc/print.h>
#include "globals.h"
#include "scanner.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
