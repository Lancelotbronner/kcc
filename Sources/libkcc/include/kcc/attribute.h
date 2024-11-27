//
//  attribute.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-08.
//

#pragma once

#include <stdint.h>

enum attribute_kind : uint8_t {
	A_UNKNOWN,
	A_DEPRECATED,
	A_FALLTHROUGH,
	A_MAYBE_UNUSED,
	A_NODISCARD,
	A_NORETURN,
	A_UNSEQUENCED,
	A_REPRODUCIBLE,
};

enum attribute_scope : uint8_t {
	SCOPE_STANDARD,
	SCOPE_KCC,
	SCOPE_CLANG,
	SCOPE_GCC,
	SCOPE_MSVC,
};

enum attribute_scopes : uint8_t {
	SCOPES_NONE = 0,
	SCOPES_ALL = 0xFF,

	SCOPES_STANDARD = 0x1,
	SCOPES_KCC = 0x2,
	SCOPES_CLANG = 0x4,
	SCOPES_GCC = 0x8,
	SCOPES_MSVC = 0x10,
};

struct attribute_syntax {
	char *spelling;
	enum attribute_kind attribute;
	enum attribute_scopes scopes;
	bool deprecated : 1;
};

struct attribute_scope_syntax {
	char *spelling;
	enum attribute_scope scope;
};

struct attribute_scope_syntax *attribute_scope(const char *identifier);
struct attribute_syntax *attribute_lookup(enum attribute_scope scope, const char *identifier);
