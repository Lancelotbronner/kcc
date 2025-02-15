//
//  token_kind.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum token_kind : unsigned char {

	// Control

	T_UNKNOWN,
	T_EOF,

	// Whitespace

	/// Whitespace produced right after a newline.
	T_INDENTATION,
	T_WHITESPACE,
	T_NEWLINE,
	T_COMMENT,
	T_DOCUMENTATION,

	// Literals

	T_STRING_LITERAL,
	T_CHAR_LITERAL,
	T_CHARS_LITERAL,
	T_FLOATING_LITERAL,
	T_INTEGER_LITERAL,

	// Literal Modifiers

	TM_UTF8,
	TM_UTF16,
	TM_UTF32,
	TM_WIDE,
	TM_FLOAT,
	TM_DOUBLE,
	TM_LDOUBLE,
	TM_DECIMAL32,
	TM_DECIMAL64,
	TM_DECIMAL128,
	TM_UNSIGNED,
	TM_LONG,
	TM_LONGLONG,
	TM_BITINT,

	// Primitives

	T_IDENTIFIER,

	// Ponctuation

	/// `(`
	T_LPAREN,
	/// `)`
	T_RPAREN,
	/// `[`
	T_LBRACKET,
	/// `]`
	T_RBRACKET,
	/// `{`
	T_LCURLY,
	/// `}`
	T_RCURLY,
	/// `;`
	T_SEMICOLON,
	/// `:`
	T_COLON,
	/// `,`
	T_COMMA,

	// Operators

	/// `+`
	T_PLUS,
	/// `++`
	T_INCREMENT,
	/// `-`
	T_SUBTRACT,
	/// `--`
	T_DECREMENT,
	/// `*`
	T_ASTERISK,
	/// `/`
	T_DIVIDE,
	/// `%`
	T_PERCENT,
	/// `~`
	T_TILDE,
	/// `!`
	T_EXCLAIM,
	/// `?`
	T_QUESTION,
	/// `==`
	T_EQ,
	/// `!=`
	T_NEQ,
	/// `>`
	T_GT,
	/// `>=`
	T_GTE,
	/// `<
	T_LT,
	/// `<=`
	T_LTE,
	/// `&`
	T_AMPERSAND,
	/// `|`
	T_PIPE,
	/// `||`
	T_LOGICAL_OR,
	/// `^`
	T_CARET,
	/// `&&`
	T_LOGICAL_AND,
	/// `.`
	T_PERIOD,
	/// `->`
	T_ARROW,
	/// `->*`
	T_ARROW_STAR,
	/// `<<`
	T_LSHIFT,
	/// `>>`
	T_RSHIFT,
	/// `#`
	T_POUND,
	/// `...`
	T_ELLIPSIS,

	/// `::`
	T_MODULE,

	// Assignment Operators

	/// `=`
	T_ASSIGN,
	/// `*=`
	T_ASSIGN_MULTIPLY,
	/// `/=`
	T_ASSIGN_DIVIDE,
	/// `%=`
	T_ASSIGN_MODULO,
	/// `+=`
	T_ASSIGN_ADD,
	/// `-=`
	T_ASSIGN_SUBTRACT,
	/// `<<=`
	T_ASSIGN_LSHIFT,
	/// `>>=`
	T_ASSIGN_RSHIFT,
	/// `&=`
	T_ASSIGN_BITWISE_AND,
	/// `^=`
	T_ASSIGN_BITWISE_XOR,
	/// `|=`
	T_ASSIGN_BITWISE_OR,

	// Keywords

	T_ALIGNAS,
	T_ALIGNOF,
	kasm,
	T_ATOMIC,
	T_AUTO,
	T_BITINT,
	T_BOOL,
	kbreak,
	kcase,
	T_CHAR,
	T_COMPLEX,
	T_CONST,
	kconstexpr,
	kcontinue,
	T_DECIMAL32,
	T_DECIMAL64,
	T_DECIMAL128,
	T_DEFAULT,
	kdo,
	T_DOUBLE,
	T_ELSE,
	kenum,
	kextern,
	kfalse,
	kfortran,
	T_FLOAT,
	T_FOR,
	T_GENERIC,
	kgoto,
	T_IF,
	kimaginary,
	T_INLINE,
	T_INT,
	T_LONG,
	T_NORETURN,
	T_NULLPTR,
	kpragma,
	kregister,
	T_RESTRICT,
	T_RETURN,
	T_SHORT,
	T_SIGNED,
	T_SIZEOF,
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
	T_UNSIGNED,
	T_VOID,
	T_VOLATILE,
	T_WHILE,

	// Preprocessor

	/// `#`
	T_DIRECTIVE,
	/// `End Of Directive`
	T_EOD,
	/// `##`
	T_STRINGIFY,

	T_IFDEF,
	T_IFNDEF,
	T_ELIF,
	T_ELIFDEF,
	T_ELIFNDEF,
	T_ENDIF,

	T_INCLUDE,
	T_EMBED,
	T_DEFINE,
	T_UNDEF,
	T_LINE,
	T_ERROR,
	T_WARNING,
	T_PRAGMA,

	TOKEN_MAX,
};

/// Returns the preprocessor keyword of the provided identifier.
enum token_kind token_preprocessor(char const *identifier, size_t length);
/// Returns the C keyword of the provided identifier.
enum token_kind token_keyword(char const *identifier, size_t length);

struct token_metadata {
	bool is_keyword : 1;
	bool is_operator : 1;
};
