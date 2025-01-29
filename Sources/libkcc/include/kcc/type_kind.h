//
//  type.h
//  kcc
//
//  Created by Christophe Bronner on 2024-07-07.
//

#pragma once

enum type_kind : unsigned char {
	TYP_UNKNOWN,

	// Special
	TYP_VOID,
	TYP_NULLPTR,

	// Integers
	TYP_BOOL,
	TYP_CHAR,
	TYP_SHORT,
	TYP_INT,
	TYP_LONG,
	TYP_LLONG,
	TYP_BITINT,

	// Floats
	TYP_FLOAT,
	TYP_DOUBLE,
	TYP_LDOUBLE,

	// Decimals
	TYP_DECIMAL32,
	TYP_DECIMAL64,
	TYP_DECIMAL128,

	// Pointers
	TYP_POINTER,
	TYP_ARRAY,
	TYP_FUNCTION,

	// Compounds
	TYP_STRUCT,
	TYP_ENUM,
	TYP_BITFIELD,
	TYP_ATOMIC,
};
