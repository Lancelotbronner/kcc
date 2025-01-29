//
//  ast_storage.h
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#pragma once

#include <kcc/types.h>

#include <stdbool.h>
#include <stddef.h>

#define AST_INLINE_CAPACITY 4

struct ast_storage {
	/// The number of elements stored.
	size_t length;
	union {
		/// The inline storage.
		ast_t storage[AST_INLINE_CAPACITY];
		struct {
			/// The allocated storage. Only valid if `length > AST_INLINE_CAPACITY`.
			ast_t *contents;
			/// The capacity of the allocated storage. Only valid if `length > AST_INLINE_CAPACITY`.
			size_t capacity;
		};
	};
};

/// Returns the length of the specified storage.
/// - Parameter storage: The storage whose length to return.
size_t ast_length(const struct ast_storage *storage);

/// Access the specified element.
/// - Parameters:
///   - storage: The storage to access.
///   - i: The element to access.
ast_t ast_at(const struct ast_storage *storage, size_t i);

/// Returns whether the specified storage is empty.
/// - Parameter storage: The storage to query.
bool ast_empty(const struct ast_storage *storage);

/// Inserts an AST node into the specified storage.
/// - Parameters:
///   - storage: The storage in which to store that node.
///   - node: The node to store.
void ast_insert(struct ast_storage *storage, ast_t node);

/// Reserves capacity.
/// - Parameters:
///   - storage: The storage to modify.
///   - capacity: The minimum number of elements this storage reserve space for.
void ast_reserve(struct ast_storage *storage, size_t capacity);
