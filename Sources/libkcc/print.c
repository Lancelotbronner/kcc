//
//  ast_print.c
//  kcc
//
//  Created by Christophe Bronner on 2024-07-20.
//

#include "print.h"

#include <stdio.h>

void print_indentation(int depth) {
	for (int i = depth - 1; i>0; i--)
		fputs("  ", stdout);
}

void token_print(struct token token) {
	fputs(token_nameof(token.kind), stdout);
}

//TODO: Rework printing
// Have functions for printing any node
// the print_node function has one switch and each node decides how to print itself

/*
 void printBT(const std::string& prefix, const BSTNode* node, bool isLeft) {
	 if (node == nullptr) return;

	 std::cout << prefix;
	 std::cout << (isLeft ? "├" : "└" );

	 // print the value of the node
	 std::cout << node->m_val << std::endl;

	 // enter the next tree level - left and right branch
	 printBT( prefix + (isLeft ? "│ " : "  "), node->m_left, true);
	 printBT( prefix + (isLeft ? "│ " : "  "), node->m_right, false);
 }
 */

void print_name(struct ast_node *node) {
	char *nameof_node = node ? ast_nameof(node->op) : "<null>";
	fputs(nameof_node ? nameof_node : "<unnamed>", stdout);
	putchar(' ');
}

bool print_header(struct ast_node *node, char *name) {
	if (name && *name) {
		fputs(name, stdout);
		putchar(':');
		putchar(' ');
	}
	if (!node) {
		print_name(node);
		putchar('\n');
		return false;
	}
	return true;
}

#pragma mark - Node Printers

void print_ast(struct ast_node *node) {
	struct tprinter printer = {};
	print_node(&printer, node, "", true);
}
