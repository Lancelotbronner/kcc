//
//  diagnostics_consumer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

#pragma once

#include <kcc/diagnostic.h>

typedef void (*diagnostics_consumer_t)(void * __nonnull data, const struct diagnostic * __nonnull const diagnostic);

void diagnostics_register(void * __nullable data, diagnostics_consumer_t __nonnull callback);
