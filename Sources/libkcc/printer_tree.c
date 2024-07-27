//
//  printer.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include "print.h"

#include <stdio.h>

void tprinter_indentation(struct tprinter *printer) {
	for (int i = 0; i < printer->depth; i++)
		fputs(printer->indentation[i], stdout);
}

void tprinter_item(struct tprinter *printer, bool leaf) {
	tprinter_indentation(printer);
	printer->indentation[printer->depth] = leaf ? "  " : "│ ";
	if (printer->depth == 0) return;
	fputs(leaf ? "└" : "├", stdout);
	fputc(' ', stdout);
}

void tprinter_indent(struct tprinter *printer) {
	printer->depth++;
}

void tprinter_unindent(struct tprinter *printer) {
	printer->indentation[printer->depth--] = 0;
}
