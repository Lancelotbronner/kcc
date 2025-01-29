//
//  print.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

#include <kcc/print.h>

#pragma mark - Tree Printer

struct tprinter {
	char *indentation[256];
	int depth;
};

void tprinter_indentation(struct tprinter *printer);

void tprinter_item(struct tprinter *printer, bool leaf);

void tprinter_indent(struct tprinter *printer);

void tprinter_unindent(struct tprinter *printer);

#pragma mark - AST Printing

void print_indentation(int depth);

void print_name(ast_t node);
bool print_header(ast_t node, char *name);
void print_node_summary(ast_t node);
void print_node_body(struct tprinter *printer, ast_t node);

void print_node(struct tprinter *printer, ast_t node, char *name, bool leaf);
void print_nodes(struct tprinter *printer, struct ast_storage *nodes, bool leaf);
