//
//  parser_context.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-10.
//

#pragma once

#include <stdint.h>

enum parser_context : uint32_t {
	CTX_DECLARATION = 0x1,
	CTX_DEFINITION = 0x2,
	CTX_MEMBER = 0x4,

	CTX_STRUCT_DECLARATION = 0x8,
	CTX_STRUCT_DEFINITION = 0x10,
	CTX_STRUCT_MEMBER = 0x20,

	CTX_UNION_DECLARATION = 0x40,
	CTX_UNION_DEFINITION = 0x80,
	CTX_UNION_MEMBER = 0x100,

	CTX_ENUM_DECLARATION = 0x200,
	CTX_ENUM_DEFINITION = 0x400,
	CTX_ENUM_MEMBER = 0x800,

	CTX_FUNCTION_DECLARATION = 0x1000,
	CTX_FUNCTION_PARAMETER = 0x2000,
	CTX_FUNCTION_DEFINITION = 0x4000,

	CTX_TYPEDEF = 0x8000,

	CTX_LOCAL_VARIABLE = 0x10000,
	CTX_GLOBAL_VARIABLE = 0x20000,

	CTX_LABEL = 0x40000,
};
