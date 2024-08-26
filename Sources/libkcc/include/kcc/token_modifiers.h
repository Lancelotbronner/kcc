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

enum token_float_modifier : unsigned char {
	FMOD_NONE,
	/// `f => float`
	FMOD_FLOAT,
	/// `d => double`
	FMOD_DOUBLE,
	/// `ld => long double`
	FMOD_LONGDOUBLE,
	/// `df => _Decimal32`
	FMOD_DECIMAL32,
	/// `dd => _Decimal64`
	FMOD_DECIMAL64,
	/// `dl => _Decimal128`
	FMOD_DECIMAL128,
};

enum token_integer_modifier : unsigned char {
	IMOD_NONE,
	/// `u => unsigned`
	IMOD_UNSIGNED,
	/// `l => long`
	IMOD_LONG,
	/// `ull => unsigned long long`
	IMOD_ULONGLONG,
	/// `ll => long long`
	IMOD_LONGLONG,
	/// `uwb => unsigned _BitInt(N)`
	IMOD_UBITINT,
	/// `wb => _BitInt(N)`
	IMOD_BITINT,
	/// `uz => usize_t`
	IMOD_USIZE,
	/// `z => size_t`
	IMOD_SIZE,
};

union token_modifiers {
	enum token_comment_modifier comment;
	enum token_encoding_modifier encoding;
	enum token_base_modifier base;
	struct {
		enum token_base_modifier : 3;
		enum token_float_modifier float_suffix : 5;
	};
	struct {
		enum token_base_modifier : 3;
		enum token_integer_modifier integer_suffix : 5;
	};
};

static_assert(sizeof(union token_modifiers) == 1, "");
