//
//  types.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-27.
//

#pragma once

typedef struct ast_node *ast_t;

typedef struct scanner *scanner_t;
typedef struct lexer *lexer_t;
typedef struct parser *parser_t;

typedef struct type const *type_t;
typedef struct type_builder *tbuilder_t;

typedef struct environment *environment_t;
typedef struct scope *scope_t;
