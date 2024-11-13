//
//  diagnostics_consumer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

#pragma once

#include <kcc/diagnostic.h>

typedef void (*diagnostics_consumer_t)(void * _Nonnull data, const struct diagnostic * _Nonnull const diagnostic);

/// Registers a callback to be executed whenever a diagnostic is emitted.
///
/// - Parameters:
///   - data: Custom data to use in the callback.
///   - callback: The callback to execute.
void diagnostics_register(void * _Nullable data, diagnostics_consumer_t _Nonnull callback);
