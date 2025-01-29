//
//  type_builder.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-12-19.
//

#pragma once

#include <kcc/types.h>

#include <stdint.h>

size_t sizeof_tbuilder();

//TODO: Accept optional diagnostic producer.
void tbuilder_init(tbuilder_t builder, scope_t scope);
void tbuilder_deinit(tbuilder_t builder);

void tbuilder_void(tbuilder_t builder);
void tbuilder_nullptr(tbuilder_t builder);

void tbuilder_unsigned(tbuilder_t builder);
void tbuilder_signed(tbuilder_t builder);

void tbuilder_char(tbuilder_t builder);
void tbuilder_short(tbuilder_t builder);
void tbuilder_int(tbuilder_t builder);
void tbuilder_long(tbuilder_t builder);

void tbuilder_float(tbuilder_t builder);
void tbuilder_double(tbuilder_t builder);
