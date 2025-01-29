//
//  type.c
//  kcc
//
//  Created by Christophe Bronner on 2024-07-07.
//

#include <kcc/type.h>

bool type_compatible(struct type target, struct type source) {
	// Void isn't compatible with anything
	if (source.kind == TYP_VOID || target.kind == TYP_VOID)
		return false;

	// Unknown (auto) is compatible with anything
	if (source.kind == TYP_UNKNOWN || target.kind == TYP_UNKNOWN)
		return true;

	// TODO: Workout promotions and compatibility rules
	return target.kind == source.kind;
}

#pragma mark - Initializers

static const struct type UNKNOWN = (struct type){ .kind = TYP_UNKNOWN };
static const struct type VOID = (struct type){ .kind = TYP_VOID };

static const struct type NULLPTR = (struct type){ .kind = TYP_NULLPTR };

static const struct type BOOL = (struct type){ .kind = TYP_BOOL };
static const struct type CHAR = (struct type){ .kind = TYP_CHAR };
static const struct type SHORT = (struct type){ .kind = TYP_SHORT };
static const struct type INT = (struct type){ .kind = TYP_INT };
static const struct type LONG = (struct type){ .kind = TYP_LONG };
static const struct type LLONG = (struct type){ .kind = TYP_LLONG };
static const struct type BITINT = (struct type){ .kind = TYP_BITINT };

static const struct type FLOAT = (struct type){ .kind = TYP_FLOAT };
static const struct type DOUBLE = (struct type){ .kind = TYP_DOUBLE };
static const struct type LDOUBLE = (struct type){ .kind = TYP_LDOUBLE };

static const struct type DECIMAL32 = (struct type){ .kind = TYP_DECIMAL32 };
static const struct type DECIMAL64 = (struct type){ .kind = TYP_DECIMAL64 };
static const struct type DECIMAL128 = (struct type){ .kind = TYP_DECIMAL128 };

type_t type_unknown() {
	return &UNKNOWN;
}

type_t type_void() {
	return &VOID;
}

type_t type_nullptr() {
	return &NULLPTR;
}

type_t type_bool() {
	return &BOOL;
}

type_t type_char() {
	return &CHAR;
}

type_t type_short() {
	return &SHORT;
}

type_t type_int() {
	return &INT;
}

type_t type_long() {
	return &LONG;
}

type_t type_llong() {
	return &LLONG;
}

struct type type_bitint(uint32_t width) {
	return (struct type){
		.kind = TYP_BITINT,
		.width = width,
	};
}

type_t type_float() {
	return &FLOAT;
}

type_t type_double() {
	return &DOUBLE;
}

type_t type_ldouble() {
	return &LDOUBLE;
}

type_t type_decimal32() {
	return &DECIMAL32;
}

type_t type_decimal64() {
	return &DECIMAL64;
}

type_t type_decimal128() {
	return &DECIMAL128;
}

#pragma mark - Properties

bool type_sign(type_t type) {
	switch (type->kind) {
	case TYP_CHAR: return true;
	case TYP_SHORT: return true;
	case TYP_INT: return true;
	case TYP_LONG: return true;
	case TYP_LLONG: return true;
	case TYP_BITINT: return true;
		//TODO: enum forward to raw type?
	default: return false;
	}
}

#pragma mark - Operators

struct type type_pointer(type_t pointee) {
	return (struct type){
		.kind = TYP_POINTER,
		.pointee = pointee,
	};
}

type_t type_pointee(type_t pointer) {
	return pointer->pointee;
}

struct type type_array(type_t element, uint32_t count) {
	return (struct type){
		.kind = TYP_ARRAY,
		.element = element,
		.count = count,
	};
}

struct type type_bitfield(type_t storage, uint32_t width) {
	return (struct type){
		.kind = TYP_BITFIELD,
		.storage = storage,
		.width = width,
	};
}

struct type type_const(type_t type) {
	struct type tmp = *type;
	tmp.is_const = true;
	return tmp;
}

struct type type_volatile(type_t type) {
	struct type tmp = *type;
	tmp.is_volatile = true;
	return tmp;
}

struct type type_restrict(type_t type) {
	struct type tmp = *type;
	tmp.is_restrict = true;
	return tmp;
}

struct type type_unqualified(type_t type) {
	struct type tmp = *type;
	tmp.is_const = false;
	tmp.is_volatile = false;
	tmp.is_restrict = false;
	return tmp;
}

struct type type_complex(type_t type) {
	struct type tmp = *type;
	tmp.is_complex = true;
	return tmp;
}

struct type type_atomic(type_t type) {
	if (type->kind == TYP_ARRAY || type->kind == TYP_FUNCTION)
		return UNKNOWN;
	return (struct type){
		.kind = TYP_ATOMIC,
		.pointee = type,
	};
}

/// Provides the compatible type for operations, if any.
/// - Parameters:
///   - subject: The type to promote.
///   - target: The dominant type, it is assumed this one won't change.
/// - Returns: A compatible type, if any.
type_t type_promote(type_t subject, type_t target);
