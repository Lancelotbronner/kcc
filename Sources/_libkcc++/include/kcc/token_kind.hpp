//
//  token.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

namespace kcc {
	enum class token_kind : unsigned char {
		unknown,
		eof,

		whitespace,
		newline,
		comment,
		documentation,

		literal_string,
		literal_char,
		literal_chars,
		literal_float,
		literal_floatX,
		literal_int,
		literal_intB,
		literal_intO,
		literal_intD,
		literal_intX,

		modifier_utf8,
		modifier_utf16,
		modifier_utf32,
		modifier_wide,
		modifier_float,
		modifier_double,
		modifier_ldouble,
		modifier_d32,
		modifier_d64,
		modifier_d128,
		modifier_unsigned,
		modifier_long,
		modifier_longlong,
		modifier_wb,

		lparen,
		rparen,
		lbracket,
		rbracket,
		lcurly,
		rcurly,
		semicolon,
		colon,

		add,
		subtract,
		multiply,
		divide,

		kalignas,
		kalignof,
		kasm,
		katomic,
		kauto,
		kbitint,
		kbool,
		kbreak,
		kcase,
		kchar,
		kcomplex,
		kconst,
		kconstexpr,
		kcontinue,
		kdecimal32,
		kdecimal64,
		kdecimal128,
		kdefault,
		kdo,
		kdouble,
		kelse,
		kenum,
		kextern,
		kfalse,
		kfloat,
		kfor,
		kgeneric,
		kgoto,
		kif,
		kimaginary,
		kinline,
		kint,
		klong,
		knoreturn,
		knullptr,
		kpragma,
		kregister,
		krestrict,
		kreturn,
		kshort,
		ksigned,
		ksizeof,
		kstatic,
		kstatic_assert,
		kstruct,
		kswitch,
		kthread_local,
		ktrue,
		ktypedef,
		ktypeof,
		ktypeof_unqual,
		kunion,
		kunsigned,
		kvoid,
		kvolatile,
		kwhile,
	};

	struct token_metadata {
		bool fkeyword : 1;
		bool foperator : 1;
	};
}
