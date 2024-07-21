//
//  type.c
//  
//
//  Created by Christophe Bronner on 2024-07-07.
//

#include <kcc/type.h>

struct type type_init(enum type_kind kind) {
	return (struct type) { .kind = kind };
}

struct type type_pointer(struct type pointee) {
	switch (pointee.kind) {
	case TYP_VOID: return type_init(TYP_VOIDP);
	default: return type_init(TYP_UNKNOWN);
	}
}

struct type type_pointee(struct type pointer) {
	switch (pointer.kind) {
	case TYP_VOIDP: return type_init(TYP_VOID);
	default: return type_init(TYP_UNKNOWN);
	}
}

struct type type_promote(struct type subject, struct type target) {
	return target;
}

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
