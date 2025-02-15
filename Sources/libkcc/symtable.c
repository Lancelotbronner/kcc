//
//  symtable.c
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#include <kcc/symtable.h>

#include <kcc/diagnostics.h>

#include <string.h>

/// Number of symbol table entries.
#define NSYMBOLS 1024

struct symtable {
	/// Position of next free global symbol slot.
	int next;
	struct symbol symbols[NSYMBOLS];
};

struct symtable *SymbolTable = &(struct symtable){};

struct symbol *symtable_find(struct symtable *symtable, char const *name) {
	for (int i = 0; i < symtable->next; i++)
		if (!strcmp(name, symtable->symbols[i].name))
			return &symtable->symbols[i];
	return nullptr;
}

struct symbol *symtable_insert(struct symtable *symtable, char const *name) {
	if (symtable->next >= NSYMBOLS)
		fatal("Ran out of space for symbol table");

	struct symbol *symbol = &symtable->symbols[symtable->next++];
	//TODO: Copy slice of the given (TokenLength) size
	*symbol = (struct symbol) { .name = strdup(name) };
	return symbol;
}
