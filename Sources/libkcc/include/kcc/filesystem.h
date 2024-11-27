//
//  filesystem.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

#pragma once

// Note: This is an interface abstracting over filesystems.
// The goal is to create virtual filesystems for testing.

typedef struct filesystem *filesystem_t;
typedef struct file *file_t;

//MARK: - File

//MARK: - Filesystem

bool filesystem_exists(char const * const path);
