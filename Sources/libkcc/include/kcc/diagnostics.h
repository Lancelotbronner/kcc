//
//  diagnostics.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <stddef.h>
#include <stdint.h>

enum diagnostic_level : uint8_t {
	DIAGNOSTIC_FATAL,
	DIAGNOSTIC_ERROR,
	DIAGNOSTIC_WARNING,
	DIAGNOSTIC_HELP,
	DIAGNOSTIC_NOTE,
};

/// Configures the severity level of the current diagnostic.
/// - Parameter level: The severity level.
void diagnostics_level(enum diagnostic_level level);

/// Configures the location of the current diagnostic.
/// - Parameters:
///   - source: The name of the source in which the issue occured.
///   - line: The line at which the issue occured.
///   - column: The column at which the issue occured.
void diagnostics_location(char *source, uint32_t line, uint32_t column);

/// Attaches a code to the current diagnostic.
/// - Parameters:
///   - group: The namespace of the code.
///   - identifier: The identifier of the code within its group.
void diagnostics_code(char group[4], uint32_t identifier);

/// Attaches a primary span to the current diagnostic.
/// - Parameters:
///   - location: The location of the span, relative to the location of the diagnostic.
///   - size: The size of the span.
///   - label: The label of the span.
void diagnostics_snippet(uint16_t location, uint16_t size, char *label);

/// Attaches a secondary span to the current diagnostic.
/// - Parameters:
///   - location: The location of the span, relative to the diagnostic.
///   - size: The size of the span.
///   - label: The label of the span.
void diagnostics_label(uint16_t location, uint16_t size, char *label);

/// Removes a span as part of an automatic fix.
/// - Parameters:
///   - location: The location of the span relative to the diagnostic.
///   - count: The number of bytes to remove.
void diagnostics_remove(uint16_t location, uint16_t count);

/// Inserts a span as part of an automatic fix.
/// - Parameters:
///   - location: The location of the span relative to the diagnostic.
///   - text: The text to insert.
void diagnostics_insert(uint16_t location, char *text);

/// Replaces a span with another as part of an automatic fix.
/// - Parameters:
///   - location: The location of the span relative to the diagnostic.
///   - count: The number of bytes to remove.
///   - text: The text to insert.
void diagnostics_replace(uint16_t location, uint16_t count, char *text);

/// Attaches the current diagnostic to the previous one.
void diagnostics_attach();

/// Emits the current diagnostic to all consumers.
void diagnostics_emit();

[[noreturn]] void fatal(char *s);
[[noreturn]] void fatals(char *s1, char *s2);
[[noreturn]] void fatald(char *s, int d);
[[noreturn]] void fatalc(char *s, int c);
[[noreturn]] void fatalt(char *s);
