//
//  token_modifiers.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

enum token_comment_modifier : unsigned char {
	COMMENT_LINE,
	COMMENT_BLOCK,
};

enum token_encoding_modifier : unsigned char {
	ENCODING_NONE,
	/// `u8`
	ENCODING_UTF8,
	/// `u`
	ENCODING_UTF16,
	/// `U`
	ENCODING_UTF32,
	/// `L`
	ENCODING_WIDE,
};

enum token_base_modifier : unsigned char {
	BASE_NONE,
	BASE_BINARY,
	BASE_DECIMAL,
	BASE_OCTAL,
	BASE_HEX,
};

enum token_floating_suffix : unsigned char {
	FSUFFIX_NONE,
	/// `f => float`
	FSUFFIX_FLOAT,
	/// `d => double`
	FSUFFIX_DOUBLE,
	/// `ld => long double`
	FSUFFIX_LONGDOUBLE,
	/// `df => _Decimal32`
	FSUFFIX_DECIMAL32,
	/// `dd => _Decimal64`
	FSUFFIX_DECIMAL64,
	/// `dl => _Decimal128`
	FSUFFIX_DECIMAL128,
};

struct token_floating_modifier {
	enum token_base_modifier base : 3;
	enum token_floating_suffix suffix : 3;
};

enum token_integer_suffix : unsigned char {
	ISUFFIX_NONE,
	/// `l => long`
	ISUFFIX_LONG,
	/// `ll => long long`
	ISUFFIX_LONGLONG,
	/// `wb => _BitInt(N)`
	ISUFFIX_BITINT,
	/// `z => size_t`
	ISUFFIX_SIZE,
};

struct token_integer_modifier {
	enum token_base_modifier base : 3;
	enum token_integer_suffix suffix : 3;
	/// `u => unsigned`
	bool is_unsigned : 1;
};

union token_modifiers {
	enum token_comment_modifier comment;
	enum token_encoding_modifier encoding;
	enum token_base_modifier base;
	struct token_floating_modifier floating;
	struct token_integer_modifier integer;
	bool digraph : 1;
};

//static_assert(sizeof(union token_modifiers) == 1);
