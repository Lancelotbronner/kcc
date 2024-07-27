//
//  tokens.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

#include <string>
#include <variant>

namespace kcc {
	struct token_identifier {
		std::string value;
	};

	struct token_integer {
		long long value;
	};

	struct token_string {
		std::string value;
	};

	enum class token_kind : unsigned short {
		unknown,

		lparen,
		rparen,
		lbracket,
		rbracket,
		lcurly,
		rcurly,
		semicolon,
		colon,

		add,
		sub,
		mul,
		div,

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

	using token_value = std::variant<token_reserved, token_identifier, token_integer, token_string>;
}
