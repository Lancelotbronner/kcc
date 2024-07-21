//
//  ast_storage.c
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#include <kcc/ast_storage.h>

#include <stdlib.h>
#include <string.h>

#define AST_INITIAL_CAPACITY 16
static_assert(AST_INITIAL_CAPACITY > AST_INLINE_CAPACITY, "Must be larger to copy inline to initial on resize.");

size_t ast_length(struct ast_storage *storage) {
	return storage->length;
}

struct ast_node *ast_at(struct ast_storage *storage, size_t i) {
	if (storage->contents)
		return storage->contents[i];
	return storage->storage[i];
}

void ast_insert(struct ast_storage *storage, struct ast_node *node) {
	if (!storage->contents) {
		if (storage->length < AST_INLINE_CAPACITY) {
			storage->storage[storage->length++] = node;
			return;
		}

		ast_reserve(storage, AST_INLINE_CAPACITY + 1);
	}

	if (storage->length == storage->capacity)
		ast_reserve(storage, storage->length + 1);
	storage->contents[storage->length++] = node;
}

void ast_reserve(struct ast_storage *storage, size_t capacity) {
	if (capacity <= AST_INLINE_CAPACITY || capacity <= storage->capacity)
		return;

	// When allocating, try minimize futur allocations by getting a larger one upfront
	if (capacity < 16)
		capacity = 16;

	// If we were already using allocated storage we can just realloc
	if (storage->length > AST_INLINE_CAPACITY) {
		storage->contents = realloc(storage->contents, capacity);
		storage->capacity = capacity;
		return;
	}

	// We have to allocate new storage and move the inline over
	struct ast_node **contents = malloc(capacity * sizeof(struct ast_node *));
	memcpy(contents, &storage->storage, storage->length);
	storage->contents = contents;
	storage->capacity = capacity;
}
