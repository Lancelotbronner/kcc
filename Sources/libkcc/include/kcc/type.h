//
//  type.h
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#pragma once

#include "type_kind.h"

#include <stdbool.h>

#pragma mark - Specialized Types

#pragma mark - Generic Type

struct type {
	enum type_kind kind;
};

struct type type_init(enum type_kind kind);

/// Produces the pointer type of the provided type.
/// - Parameter pointee: The type of the pointee.
struct type type_pointer(struct type pointee);

/// Produces the pointee type of the provided pointer type.
/// - Parameter pointer: The type of the pointer.
/// - Returns: The pointee type if applicable, `TYP_UNKNOWN` otherwise.
struct type type_pointee(struct type pointer);

/// Provides the compatible type for operations, if any.
/// - Parameters:
///   - subject: The type to promote.
///   - target: The dominant type, it is assumed this one won't change.
/// - Returns: A compatible type, if any.
struct type type_promote(struct type subject, struct type target);

bool type_compatible(struct type target, struct type source);
