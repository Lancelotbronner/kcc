//
//  token.h
//  
//
//  Created by Christophe Bronner on 2024-07-04.
//

#pragma once

#include "token_kind.h"
#include "token_modifiers.h"

struct token_flags {
	bool line_start : 1;
	//TODO: rename to trivia_leading
	bool leading_space : 1;
};

struct token {
	enum token_kind kind;
	union token_modifiers modifiers;
	struct token_flags flags;
};
