/*-------------------------------------------------------------------------
  common.h - include common header files

  Copyright (C) 2000, KEvin Vigor

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2, or (at your option) any
  later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
-------------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMMON_H
#define COMMON_H

#if defined(__APPLE__) && (__MACH__)
#ifdef _G
#undef _G
#endif
#endif

/* C++ incompatible heared files */
#ifdef __cplusplus
/* TODO: the extern "C" should be moved to each header file */
extern "C" {
#endif

#include "BBlock.h"
#include "asm.h"
#include "ast.h"
#include "bitv.h"
#include "cflow.h"
#include "cse.h"
#include "dflow.h"
#include "globl.h"
#include "glue.h"
#include "hasht.h"
#include "icode.h"
#include "label.h"
#include "loop.h"
#include "lrange.h"
#include "mem.h"
#include "opt.h"
#include "peeph.h"
#include "ptropt.h"
#include "set.h"
#include "system.h"
#include "util.h"
#include <y.tab.h>

#include "port.h"

#include "newalloc.h"

#ifdef __cplusplus
}
#endif

/* C++ compatible header files */
#include "gen.h"

#endif
