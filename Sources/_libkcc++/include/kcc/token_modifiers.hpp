//
//  token_modifiers.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-29.
//

#pragma once

namespace kcc {

	enum class token_comment_modifier : unsigned char {
		line,
		block,
	};

	enum class token_encoding_modifier : unsigned char {
		none,
		utf8,
		utf16,
		utf32,
		wide,
	};

	enum class token_base_modifier : unsigned char {
		none,
		binary,
		decimal,
		octal,
		hex,
	};

	enum class token_float_modifier : unsigned char {
		none,
		/// `float`
		f,
		/// `double`
		d,
		/// `long double`
		ld,
		/// `_Decimal32`
		df,
		/// `_Decimal64`
		dd,
		/// `_Decimal128`
		dl,
	};

	enum class token_integer_modifier : unsigned char {
		none,
		/// `unsigned`
		u,
		/// `long`
		l,
		/// `unsigned long long`
		ull,
		/// `long long`
		ll,
		/// `unsigned _BitInt(N)`
		uwb,
		/// `_BitInt(N)`
		wb,
		/// `usize_t`
		uz,
		/// `size_t`
		z,
	};

	union token_modifiers {
		token_comment_modifier comment;
		token_encoding_modifier encoding;
		token_base_modifier base;
		struct {
			token_base_modifier : 3;
			token_float_modifier float_suffix : 5;
		};
		struct {
			token_base_modifier : 3;
			token_integer_modifier integer_suffix : 5;
		};
	};
}

static_assert(sizeof(kcc::token_modifiers) == 1);
