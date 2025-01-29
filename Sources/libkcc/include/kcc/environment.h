//
//  environment.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-12-19.
//

#pragma once

#include <stdint.h>

size_t sizeof_environment();

//TODO: execution environment: char's default sign and size
void environment_char(environment_t environment, unsigned char size, bool is_unsigned);
//TODO: execution environment: default size of integer types (0 if unsupported, except int)
void environment_short(environment_t environment, unsigned char size);
void environment_int(environment_t environment, unsigned char size);
void environment_long(environment_t environment, unsigned char size);
void environment_llong(environment_t environment, unsigned char size);
