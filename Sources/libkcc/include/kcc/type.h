//
//  type.h
//  kcc
//
//  Created by Christophe Bronner on 2024-07-07.
//

#pragma once

#include <kcc/type_kind.h>
#include <kcc/types.h>

#include <stdbool.h>
#include <stdint.h>

struct type {
	union {
		/// The element type of an array.
		type_t element;
		/// The pointee type of a pointer.
		type_t pointee;
		/// The storage type of a bitfield.
		type_t storage;
		// struct_t struct_declaration;
		// enum_t enum_declaration;
	};
	union {
		/// The size of an array in elements.
		uint32_t count;
		/// The size of a `_BitInt` or bitfield in bits.
		uint32_t width;
	};
	/// The category of a type.
	enum type_kind kind;
	/// Whether an integer is unsigned.
	bool is_unsigned : 1;
	/// Whether an integer is signed.
	bool is_signed : 1;
	/// Whether a floating point is unsigned.
	bool is_complex : 1;
	/// Whether a type is constant.
	bool is_const : 1;
	/// Whether a type is volatile.
	bool is_volatile : 1;
	/// Whether a pointer or array is restricted.
	bool is_restrict : 1;
};

struct type type_lookup(enum type_kind kind);

bool type_compatible(struct type target, struct type source);

#pragma mark - Initializers

/// Returns the _unknown_ type.
type_t type_unknown();

/// Returns the `void` type.
type_t type_void();

/// Returns the `nullptr_t` type.
type_t type_nullptr();

/// Returns the `bool` type.
type_t type_bool();

/// Returns the `char` type.
type_t type_char();

/// Returns the `short` type.
type_t type_short();

/// Returns the `int` type.
type_t type_int();

/// Returns the `long` type.
type_t type_long();

/// Returns the `long long` type.
type_t type_llong();

/// Produce a `_BitInt(size)` type.
/// - Parameter size: The size of the integer in bits.
struct type type_bitint(uint32_t size);

/// Returns the `float` type.
type_t type_float();

/// Returns the `double` type.
type_t type_double();

/// Returns the `long double` type.
type_t type_ldouble();

/// Returns the `_Decimal32` type.
type_t type_decimal32();

/// Returns the `_Decimal64` type.
type_t type_decimal64();

/// Returns the `_Decimal128` type.
type_t type_decimal128();

#pragma mark - Properties

/// Evaluates the alignment of a type within its environment.
/// - Parameters:
///   - type: The type to evaluate.
///   - environment: The environment in which to evaluate the type.
size_t type_alignment(type_t type, environment_t environment);

/// Evaluates the size of a type within its environment.
/// - Parameters:
///   - type: The type to evaluate.
///   - environment: The environment in which to evaluate the type.
size_t type_size(type_t type, environment_t environment);

/// Whether a type can be signed or unsigned.
/// - Parameter type: The type to query.
bool type_sign(type_t type);

#pragma mark - Operators

/// Produces the pointer type of the provided type.
/// - Parameter pointee: The type of the pointee.
struct type type_pointer(type_t pointee);

/// Produces the pointee type of the provided pointer type.
/// - Parameter pointer: The type of the pointer.
/// - Returns: The pointee type if applicable, `TYP_UNKNOWN` otherwise.
type_t type_pointee(type_t pointer);

/// Produces an array type with the specified size.
/// - Parameters:
///   - element: The type of the array's element.
///   - size: The size of the array in elements.
/// - Returns: The array type.
struct type type_array(type_t element, uint32_t size);

/// Produces a bitfield type with the specified size.
/// - Parameters:
///   - storage: The type of the bitfield's storage.
///   - size: The size of the bitfield in bits.
/// - Returns: The bitfield type.
struct type type_bitfield(type_t storage, uint32_t size);

struct type type_const(type_t type);
struct type type_volatile(type_t type);
struct type type_restrict(type_t type);
struct type type_unqualified(type_t type);

/// Produces an atomic variant of a base type.
/// - Parameter type: The base type.
struct type type_atomic(type_t type);

/// Produces a complex variant of a base floating-point type.
/// - Parameter type: The base floating point type.
struct type type_complex(type_t type);

/// Provides the compatible type for operations, if any.
/// - Parameters:
///   - subject: The type to promote.
///   - target: The dominant type, it is assumed this one won't change.
/// - Returns: A compatible type, if any.
type_t type_promote(type_t subject, type_t target);
