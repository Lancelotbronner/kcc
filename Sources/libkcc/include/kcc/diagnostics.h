//
//  diagnostics.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

[[noreturn]] void fatal(char *s);
[[noreturn]] void fatals(char *s1, char *s2);
[[noreturn]] void fatald(char *s, int d);
[[noreturn]] void fatalc(char *s, int c);
[[noreturn]] void fatalt(char *s);
