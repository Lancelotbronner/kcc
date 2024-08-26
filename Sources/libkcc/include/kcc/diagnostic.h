//
//  diagnostic.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-10.
//

#pragma once

#include <kcc/diagnostics.h>

#include <stdint.h>

struct diagnostic_code {
	char group[4];
	uint32_t identifier;
};

struct diagnostic_span {
	/// Describes the span.
	char *label;
	/// The location of the span, relative to the diagnostic's location.
	uint16_t location;
	/// The size of the span, in characters
	uint16_t size;
	uint32_t reserved;
};

enum diagnostic_fix_flags : uint8_t {
	DIAGNOSTIC_INSERT = 0x1,
	DIAGNOSTIC_REMOVE = 0x2,
};

struct diagnostic_fix {
	struct {
		char *text;
		uint16_t location;
	} insertion;
	struct {
		uint16_t location;
		uint16_t count;
	} removal;
	union {
		enum diagnostic_fix_flags flags;
		struct {
			/// Whether this insertion is active.
			bool insert : 1;
			/// Whether this removal is active.
			bool remove : 1;
		};
	};
	uint8_t reserved;
};

#define DIAGNOSTIC_LABELS 3
#define DIAGNOSTIC_FIXES 2

struct diagnostic {
	/// Helps users get more information about the current error through an extended description of the problem in the error code index.
	///
	/// Not all diagnostic have a code.
	/// For example, diagnostics created by lints don't have one.
	struct diagnostic_code code;

	/// Name of the file or virtual buffer where the code is located.
	char *source;

	/// Line where the primary issue arised.
	uint32_t line;

	/// Column where the primary issue arised.
	uint32_t column;

	/// The main description of the problem.
	///
	/// It should be general and able to stand on its own, so that it can make sense even in isolation.
	char *message;

	/// The primary span of the diagnostic, has emphasis.
	///
	/// Primary spans should have enough text to describe the problem in such a way that if it were the only thing being displayed (for example, in an IDE) it would still make sense.
	/// Because it is "spatially aware" (it points at the code), it can generally be more succinct than the error message.
	struct diagnostic_span primary;

	/// Additional spans for details.
	///
	/// If cluttered output can be foreseen in cases when multiple span labels overlap, it is a good idea to tweak the output appropriately.
	/// For example, the if/else arms have incompatible types error uses different spans depending on whether the arms are all in the same line, if one of the arms is empty and if none of those cases applies.
	struct diagnostic_span secondary[DIAGNOSTIC_LABELS];

	/// Automatic correction of the issue.
	struct diagnostic_fix fix[DIAGNOSTIC_FIXES];

	/// Indicates the severity of the message.
	enum diagnostic_level level : 3;

	/// Whether this diagnostic is attached to the previous one as a sub-diagnostic.
	bool attached : 1;
};
