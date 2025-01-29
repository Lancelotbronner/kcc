//
//  tbuilder_builder.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-12-19.
//

#include "type_builder.h"

#include <kcc/type.h>

size_t sizeof_typebuilder() {
	return sizeof(struct type_builder);
}

void typebuilder_init(tbuilder_t builder, scope_t scope) {
	*builder = (struct type_builder){
		.scope = scope
	};
}

//MARK: - Utilities

static bool tbuilder_sign(enum type_kind kind) {
	switch (kind) {
	case TYP_CHAR: return true;
	case TYP_SHORT: return true;
	case TYP_INT: return true;
	case TYP_LONG: return true;
	case TYP_LLONG: return true;
	case TYP_BITINT: return true;
	default: return false;
	}
}

static void diagnose_duplicate_specifier(tbuilder_t builder) {
	//TODO: diagnose warning: duplicate specifier
}

static void diagnose_cannot_be_both_signed_and_unsigned(tbuilder_t builder) {
	//TODO: diagnose error: cannot be both signed and unsigned
}

static void diagnose_tbuilder_cannot_be_signed_or_unsigned(tbuilder_t builder) {
	//TODO: diagnose error: type cannot be signed or unsigned
}

static void diagnose_tbuilder_already_specified(tbuilder_t builder, enum type_kind type) {
	//TODO: diagnose error: type already specified
}

//MARK: - Specifiers

static void specifier_type(tbuilder_t builder, enum type_kind type) {
	if (builder->type.kind == type)
		return diagnose_duplicate_specifier(builder);

	if (builder->type.kind != TYP_UNKNOWN)
		return diagnose_tbuilder_already_specified(builder, type);

	builder->type.kind = type;

	// validate the new type according with previous information
	bool has_sign = builder->specifiers.is_unsigned || builder->specifiers.is_signed;
	if (has_sign && !tbuilder_sign(type))
		diagnose_tbuilder_cannot_be_signed_or_unsigned(builder);
}

void tbuilder_void(tbuilder_t builder) {
	specifier_type(builder, TYP_VOID);
}

void tbuilder_nullptr(tbuilder_t builder) {
	specifier_type(builder, TYP_NULLPTR);
}

void tbuilder_unsigned(tbuilder_t builder) {
	if (!tbuilder_sign(builder->type.kind))
		return diagnose_tbuilder_cannot_be_signed_or_unsigned(builder);

	if (builder->specifiers.is_unsigned)
		return diagnose_duplicate_specifier(builder);

	if (builder->specifiers.is_signed)
		return diagnose_cannot_be_both_signed_and_unsigned(builder);

	builder->specifiers.is_unsigned = true;
}

void tbuilder_signed(tbuilder_t builder) {
	if (!tbuilder_sign(builder->type.kind))
		return diagnose_tbuilder_cannot_be_signed_or_unsigned(builder);

	if (builder->specifiers.is_signed)
		return diagnose_duplicate_specifier(builder);

	if (builder->specifiers.is_unsigned)
		return diagnose_cannot_be_both_signed_and_unsigned(builder);

	builder->specifiers.is_signed = true;
}

void tbuilder_char(tbuilder_t builder) {
	specifier_type(builder, TYP_CHAR);
}

void tbuilder_short(tbuilder_t builder) {
	specifier_type(builder, TYP_SHORT);
}

void tbuilder_int(tbuilder_t builder) {
	specifier_type(builder, TYP_INT);
}

void tbuilder_long(tbuilder_t builder) {
	specifier_type(builder, TYP_LONG);
}

void tbuilder_float(tbuilder_t builder) {
	specifier_type(builder, TYP_FLOAT);
}

void tbuilder_double(tbuilder_t builder) {
	specifier_type(builder, TYP_DOUBLE);
}
