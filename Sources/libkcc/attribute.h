//
//  attribute.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-10.
//

#include <kcc/attribute.h>

#include <stdint.h>

enum attribute_context : uint8_t {
	ACTX_DEFINITION = 0x1,
	ACTX_MEMBER = 0x2,
	ACTX_STATEMENT = 0x4,
//	ACTX_DECLARATION,
//	ACTX_EXPRESSION = 0x10,
	ACTX_FUNCTION = 0x8,
//	ACTX_STRUCT = ACTX_DECLARATION,
//	ACTX_STRUCT_MEMBER = ACTX_MEMBER,
//	ACTX_UNION = ACTX_DECLARATION,
//	ACTX_UNION_MEMBER = ACTX_MEMBER,
//	ACTX_ENUM = ACTX_DECLARATION,
//	ACTX_ENUM_MEMBER = ACTX_MEMBER,
	ACTX_TYPEDEF = 0x10,
	ACTX_OBJECT = 0x20,
	ACTX_LABEL = 0x40,
};

struct attribute_metadata {
	char *name;
	enum attribute_context context;
};

struct attribute_scope_metadata {
	char *name;
};
