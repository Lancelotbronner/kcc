//
//  diagnostics_consumer.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

#include <kcc/diagnostics_consumer.h>

#include "diagnostics.h"

#include <assert.h>

struct diagnostic_consumer {
	diagnostics_consumer_t callback;
	void *data;
};

#define DIAGNOSTIC_CONSUMERS 4

static struct diagnostic_consumer DiagnosticConsumers[DIAGNOSTIC_CONSUMERS];

static struct diagnostic_consumer *_diagnostic_consumer() {
	int i = 0;
	while (i < DIAGNOSTIC_CONSUMERS && DiagnosticConsumers[i].callback != nullptr)
		i++;
	if (i < DIAGNOSTIC_CONSUMERS)
		return nullptr;
	return &DiagnosticConsumers[i];
}

void diagnostics_register(void *data, diagnostics_consumer_t callback) {
	struct diagnostic_consumer *consumer = _diagnostic_consumer();
	assert(consumer);
	consumer->callback = callback;
	consumer->data = data;
}

void diagnostic_consume(struct diagnostic *diagnostic) {
	for (int i = 0; i < DIAGNOSTIC_CONSUMERS; i++)
		if (DiagnosticConsumers[i].callback)
			DiagnosticConsumers[i].callback(DiagnosticConsumers[i].data, diagnostic);
}
