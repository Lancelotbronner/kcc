//
//  args.c
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include <kcc/args.h>

#include "globals.h"
#include "lexer.h"

void kcc_infile(FILE *file) {
	Infile = file;
}

void kcc_inmem(const void *buf, size_t size) {
	Infile = fmemopen((void*)buf, size, "r");
	scanner_init(Scanner, buf, size);
}

void kcc_outfile(FILE *file) {
	Outfile = file;
}

size_t kcc_line() {
	return Line;
}

size_t kcc_column() {
	return Column;
}
