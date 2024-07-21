//
//  symtable.h
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "symbol.h"

struct symtable;

/// The global symbol table.
struct symtable *SymbolTable;

/// Determines if the symbol `name` is in the global symbol table.
/// - Parameter name: The name of the symbol.
/// - Returns: A pointer to the symbol if found, null otherwise.
struct symbol *symtable_find(struct symtable *table, char *name);

/// Add a global symbol to the symbol table.
/// - Returns: A reference to the symbol.
struct symbol *symtable_insert(struct symtable *table, char *name);
