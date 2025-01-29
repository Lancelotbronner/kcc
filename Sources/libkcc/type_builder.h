//
//  type_builder.h
//  kcc2
//
//  Created by Christophe Bronner on 2025-01-20.
//

#pragma once

#include <kcc/type_builder.h>

#include <kcc/type.h>

struct type_specifiers {
	// Special
	bool is_void : 1;
	bool is_nullptr : 1;
	// Integers
	bool is_unsigned : 1;
	bool is_signed : 1;
	// Floating Points
	bool is_complex : 1;
	// Pointers
	bool is_restrict : 1;
	// General
	bool is_const : 1;
	bool is_volatile : 1;
	bool is_atomic : 1;
};

struct type_builder {
	scope_t scope;
	struct type type;
	struct type_specifiers specifiers;
};
