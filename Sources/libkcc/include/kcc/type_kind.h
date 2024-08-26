//
//  type.h
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#pragma once

enum type_kind : unsigned char {
	TYP_UNKNOWN,

	// Special
	TYP_VOID,

	// Integers
	TYP_BOOL,
	TYP_CHAR,
	TYP_SHORT,
	TYP_INT,
	TYP_LONG,
	TYP_LONGLONG,
	TYP_BITINT,

	// Floats
	TYP_FLOAT,
	TYP_DOUBLE,
	TYP_LONGDOUBLE,

	// Decimals
	TYP_DECIMAL32,
	TYP_DECIMAL64,
	TYP_DECIMAL128,

	// Pointers
	TYP_VOIDP,
	TYP_NULLP,
};
