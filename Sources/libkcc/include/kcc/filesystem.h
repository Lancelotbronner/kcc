//
//  filesystem.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

#pragma once

typedef struct filesystem *filesystem_t;
typedef struct file *file_t;

//MARK: - File

//MARK: - Filesystem

bool filesystem_exists(char const * const path);
