//
//  symbol.h
//
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "type.h"

#include <stdbool.h>

/// Symbol table structure.
struct symbol {
	/// Name of a symbol.
	char *name;
	/// The type of a symbol.
	struct type type;
	/// Whether the symbol was declared.
	bool declared : 1;
	/// Whether the symbol was defined.
	bool defined : 1;
	/// Whether the symbol was used.
	bool used : 1;
};
