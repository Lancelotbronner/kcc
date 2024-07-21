//
//  kcc.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include <stdio.h>

/// Current line number.
size_t Line;

/// Current column number.
size_t Column;

/// Input file.
FILE *Infile;

/// Output file.
FILE *Outfile;
