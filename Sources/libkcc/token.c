//
//  token.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

#include <kcc/token_kind.h>

#ifdef _MSC_VER
#include <stdlib.h> // _byteswap_ulong
#endif

enum token_kind token_preprocessor(char const *identifier, size_t length) {
	//	if
 // elif
 // else
 // endif
 // ifdef
 // ifndef
 // elifdef (C23)
 // elifndef (C23)
 // define
 // undef
 // include
 // embed (C23)
 // line
 // error
 // warning (C23)
 // pragma
 // defined
 // __has_include (C23)
 // __has_embed (C23)
 // __has_c_attribute (C23)
	return T_IDENTIFIER;
}

static inline uint32_t bswap32(uint32_t value) {
#ifdef __GNUC__ // also works with Clang
	return __builtin_bswap32(value);
#elifdef _MSC_VER
	return _byteswap_ulong(value);
#endif
}

enum token_kind token_keyword(char const *identifier, size_t length) {
	// just.. don't look at the following switch, please
	// this is why I want multi-character literals to be defined:
	// I just want to reinterpret as uint64 or uint128 (or _BitInt?) and switch away!
	uint32_t token = bswap32(*(uint32_t*)identifier);
	uint32_t token2 = bswap32(*(uint32_t*)(identifier+4));
	uint32_t token3 = bswap32(*(uint32_t*)(identifier+8));
	uint32_t token4 = bswap32(*(uint32_t*)(identifier+12));
	// for what its worth: its faster than a bunch of strcmp
	switch (length) {
	case 2: token >>= 16;
		switch (token) {
		case '\0\0do': return kdo;
		case '\0\0if': return T_IF;
		}
		break;
	case 3: token >>= 8;
		switch (token) {
		case '\0for': return T_FOR;
		case '\0int': return T_INT;
		case '\0asm': return kasm;
		}
		break;
	case 4:
		switch (token) {
		case 'auto': return T_AUTO;
		case 'bool': return T_BOOL;
		case 'case': return kcase;
		case 'else': return T_ELSE;
		case 'enum': return kenum;
		case 'goto': return kgoto;
		case 'long': return T_LONG;
		case 'true': return ktrue;
		case 'void': return T_VOID;
		}
		break;
	case 5: token2 >>= 24;
		switch (token) {
		case 'brea': if (token2 == 'k') return kbreak; else break;
		case 'cons': if (token2 == 't') return T_CONST; else break;
		case 'fals': if (token2 == 'e') return kfalse; else break;
		case 'floa': if (token2 == 't') return T_FLOAT; else break;
		case 'shor': if (token2 == 't') return T_SHORT; else break;
		case 'unio': if (token2 == 'n') return kunion; else break;
		case 'whil': if (token2 == 'e') return T_WHILE; else break;
		case '_Boo': if (token2 == 'l') return T_BOOL; else break;
		}
	case 6: token2 >>= 16;
		switch (token) {
		case 'doub': if (token2 == '\0\0le') return T_DOUBLE; else break;
		case 'exte': if (token2 == '\0\0rn') return kextern; else break;
		case 'inli': if (token2 == '\0\0ne') return T_INLINE; else break;
		case 'retu': if (token2 == '\0\0rn') return T_RETURN; else break;
		case 'sign': if (token2 == '\0\0ed') return T_SIGNED; else break;
		case 'size': if (token2 == '\0\0of') return T_SIZEOF; else break;
		case 'stat': if (token2 == '\0\0ic') return kstatic; else break;
		case 'stru': if (token2 == '\0\0ct') return kstruct; else break;
		case 'swit': if (token2 == '\0\0ch') return kswitch; else break;
		case 'type': if (token2 == '\0\0of') return ktypeof; else break;
		}
		break;
	case 7: token2 >>= 8;
		switch (token) {
		case 'alig':
			switch (token2) {
			case '\0nas': return T_ALIGNAS;
			case '\0nof': return T_ALIGNOF;
			default: break;
			} break;
		case 'defa': if (token2 == '\0ult') return T_DEFAULT; else break;
		case 'null': if (token2 == '\0ptr') return T_NULLPTR; else break;
		case 'type': if (token2 == '\0def') return ktypedef; else break;
		case '_Ato': if (token2 == '\0mic') return T_ATOMIC; else break;
		case '_Bit': if (token2 == '\0Int') return T_BITINT; else break;
		case '_Pra': if (token2 == '\0gma') return T_PRAGMA; else break;
		case 'fort': if (token2 == '\0ran') return kfortran; else break;
		}
		break;
	case 8:
		switch (token) {
		case 'regi': if (token2 == 'ster') return kregister; else break;
		case 'rest': if (token2 == 'rict') return T_RESTRICT; else break;
		case 'unsi': if (token2 == 'gned') return T_UNSIGNED; else break;
		case 'vola': if (token2 == 'tile') return T_VOLATILE; else break;
		case '_Ali':
			switch (token2) {
			case 'gnas': return T_ALIGNAS;
			case 'gnof': return T_ALIGNOF;
			} break;
		case '_Com': if (token2 == 'plex') return T_COMPLEX; else break;
		case '_Gen': if (token2 == 'eric') return T_GENERIC; else break;
		}
		break;
	case 9: token3 >>= 24;
		if (token == '_Nor' && token2 == 'etur' && token3 == 'n')
			return T_NORETURN;
		break;
	case 10: token3 >>= 16;
		if (token == '_Dec' && token2 == 'imal')
			switch (token3) { // oh god '\0\032' forms an octal escape and means 0x00 0x1A nooooo
			case '\0\0\x33\x32': return T_DECIMAL32;
			case '\0\0\x36\x34': return T_DECIMAL64;
			}
		else if (token == '_Ima' && token2 == 'gina' && token3 == '\0\0ry')
			return kimaginary;
		break;
	case 11: token3 >>= 8; // why can't I do '\0' '128'???
		if (token == '_Dec' && token2 == 'imal' && token3 == '\0\x31\x32\x38')
			return T_DECIMAL128;
		break;
	case 12:
		if (token == 'thre' && token2 == 'ad_l' && token3 == 'ocal')
			return kthread_local;
		break;
	case 13:  token4 >>= 24; // aaaaarrrgh
		if (token == 'stat' && token2 == 'ic_a' && token3 == 'sser' && token4 == 't')
			return kstatic_assert;
		else if (token == 'type' && token2 == 'of_u' && token3 == 'nqua' && token4 == 'l')
			return ktypeof_unqual;
		else if (token == '_Thr' && token2 == 'ead_' && token3 == 'loca' && token4 == 'l')
			return kthread_local;
		break;
	case 14: token4 >>= 16; // whaaaaa
		if (token == '_Sta' && token2 == 'tic_' && token3 == 'asse' && token4 == '\0\0rt')
			return kstatic_assert;
		break;
	}
	return T_IDENTIFIER;
}
