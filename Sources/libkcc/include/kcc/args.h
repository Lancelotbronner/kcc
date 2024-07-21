//
//  args.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <stdio.h>

/// Configures the input file of the compiler.
void kcc_infile(FILE *file);

/// Configures the input stream of the compiler.
void kcc_inmem(const void *buf, size_t size);

/// Configures the output file of the compiler.
void kcc_outfile(FILE *file);

/// Current line number.
size_t kcc_line();

/// Current column number.
size_t kcc_column();
