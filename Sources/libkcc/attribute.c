//
//  attribute.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-08.
//

#include "attribute.h"

#include <string.h>

static struct attribute_metadata AttributeMetadata[] = {
	[A_NODISCARD] = {
		.name = "nodiscard",
		.context = ACTX_FUNCTION | ACTX_DEFINITION,
	},
	[A_MAYBE_UNUSED] = {
		.name = "maybe_unused",
		.context = ACTX_DEFINITION | ACTX_MEMBER | ACTX_FUNCTION | ACTX_LABEL,
	},
	[A_DEPRECATED] = {
		.name = "deprecated",
		.context = ACTX_DEFINITION | ACTX_TYPEDEF | ACTX_OBJECT | ACTX_MEMBER | ACTX_FUNCTION,
	},
	[A_FALLTHROUGH] = {
		.name = "fallthrough",
		.context = ACTX_STATEMENT,
	},
	[A_NORETURN] = {
		.name = "noreturn",
		.context = ACTX_FUNCTION,
	}
};

static struct attribute_syntax AttributeSyntax[] = {
	{ "nodiscard", A_NODISCARD, SCOPES_STANDARD },
	{ "maybe_unused", A_MAYBE_UNUSED, SCOPES_STANDARD },
	{ "deprecated", A_DEPRECATED, SCOPES_STANDARD },
	{ "fallthrough", A_FALLTHROUGH, SCOPES_STANDARD },
	{ "noreturn", A_NORETURN, SCOPES_STANDARD },
	{ "_NoReturn", A_NORETURN, SCOPES_STANDARD, .deprecated = true },
};

static struct attribute_scope_metadata ScopeMetadata[] = {
	[SCOPE_STANDARD] = {
		.name = "standard",
	},
	[SCOPE_KCC] = {
		.name = "kcc",
	},
	[SCOPE_CLANG] = {
		.name = "clang",
	},
	[SCOPE_GCC] = {
		.name = "gcc",
	},
	[SCOPE_MSVC] = {
		.name = "msvc",
	},
};

static struct attribute_scope_syntax ScopeSyntax[] = {
	{ "kcc", SCOPE_KCC },
	{ "clang", SCOPE_CLANG },
	{ "gcc", SCOPE_GCC },
	{ "msvc", SCOPE_MSVC },
};

struct attribute_scope_syntax *attribute_scope(const char *identifier) {
	for (int i = 0; i < sizeof(ScopeSyntax) / sizeof(struct attribute_scope_syntax); i++)
		if (strcmp(identifier, ScopeSyntax[i].spelling) == 0)
			return &ScopeSyntax[i];
	return nullptr;
}

struct attribute_syntax *attribute_lookup(enum attribute_scope scope, const char *identifier) {
	enum attribute_scopes scope_mask = 1 << scope;
	for (int i = 0; i < sizeof(AttributeSyntax) / sizeof(struct attribute_syntax); i++)
		if (AttributeSyntax[i].scopes & scope_mask && strcmp(identifier, AttributeSyntax[i].spelling) == 0)
			return &AttributeSyntax[i];
	return nullptr;
}
