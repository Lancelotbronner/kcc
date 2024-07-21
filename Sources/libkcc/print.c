//
//  ast_print.c
//  kcc
//
//  Created by Christophe Bronner on 2024-07-20.
//

#include <kcc/print.h>

#include <stdio.h>

#define BRANCH "├"
#define LEAF "└"

static void printer_indentation(int depth) {
	for (int i = depth - 1; i>0; i--)
		fputs("  ", stdout);
}

void token_print(struct token token) {
	fputs(token_nameof(token.kind), stdout);
}

//TODO: Rework printing
// Have functions for printing any node
// the print_node function has one switch and each node decides how to print itself

static void ast_print_name(struct ast_node *node) {
	char *nameof_node = node ? ast_nameof(node->op) : "<null>";
	fputs(nameof_node ? nameof_node : "<unnamed>", stdout);
	putchar(' ');
}

static void ast_print_node(struct ast_node *node, int depth, char *name, char *decoration) {
	printer_indentation(depth++);
	if (*decoration) {
		fputs(decoration, stdout);
		putchar(' ');
	}
	if (*name) {
		fputs(name, stdout);
		putchar(':');
		putchar(' ');
	}

	if (!node) {
		ast_print_name(node);
		putchar('\n');
		return;
	}

	switch (node->op) {
		// Literal Expressions
	case N_INTEGER_LITERAL:
		printf("%zu", node->integer_literal.value);
		break;
		// Primary Expressions
	case N_IDENTIFIER:
		printf("`%s`", node->identifier.symbol->name);
		break;
		// Unary Expressions
	case N_ADDRESSOF:
	case N_DEREFERENCE:
	case N_POSITIVE:
	case N_NEGATIVE:
	case N_BITWISE_NOT:
	case N_LOGICAL_NOT:
	case N_INCREMENT_PREFIX:
	case N_DECREMENT_PREFIX:
		ast_print_name(node);
		//TODO: print type
		break;
		// Binary Expressions
	case N_MULTIPLY:
	case N_DIVIDE:
	case N_MODULO:
	case N_ADD:
	case N_SUBTRACT:
	case N_LSHIFT:
	case N_RSHIFT:
	case N_GT:
	case N_GTE:
	case N_LT:
	case N_LTE:
	case N_EQ:
	case N_NEQ:
	case N_BITWISE_AND:
	case N_BITWISE_XOR:
	case N_BITWISE_OR:
	case N_LOGICAL_AND:
	case N_LOGICAL_OR:
	case N_ASSIGN:
	case N_ASSIGN_MULTIPLY:
	case N_ASSIGN_DIVIDE:
	case N_ASSIGN_MODULO:
	case N_ASSIGN_ADD:
	case N_ASSIGN_SUBTRACT:
	case N_ASSIGN_LSHIFT:
	case N_ASSIGN_RSHIFT:
	case N_ASSIGN_BITWISE_AND:
	case N_ASSIGN_BITWISE_XOR:
	case N_ASSIGN_BITWISE_OR:
		ast_print_name(node);
		//TODO: print type
		break;
	case N_CAST:
		//TODO: print type
		printf("(%s)", node->cast_expression.type_tree->identifier.symbol->name);
		break;
	default:
		ast_print_name(node);
		break;
	}

	putchar('\n');

	switch (node->op) {
	case N_CAST:
		ast_print_node(node->cast_expression.type_tree, depth, "type", BRANCH);
		ast_print_node(node->cast_expression.operand, depth, "", LEAF);
		break;
	case N_IF:
	case N_CONDITIONAL:
		ast_print_node(node->if_statement.condition, depth, "condition", BRANCH);
		ast_print_node(node->if_statement.then_tree, depth, "then", node->if_statement.else_tree ? BRANCH : LEAF);
		if (node->if_statement.else_tree)
			ast_print_node(node->if_statement.else_tree, depth, "else", LEAF);
		break;
		// Unary Expressions
	case N_ADDRESSOF:
	case N_DEREFERENCE:
	case N_POSITIVE:
	case N_NEGATIVE:
	case N_BITWISE_NOT:
	case N_LOGICAL_NOT:
	case N_INCREMENT_PREFIX:
	case N_DECREMENT_PREFIX:
		ast_print_node(node->unary_expression.operand, depth, "", LEAF);
		break;
		// Binary Expressions
	case N_MULTIPLY:
	case N_DIVIDE:
	case N_MODULO:
	case N_ADD:
	case N_SUBTRACT:
	case N_LSHIFT:
	case N_RSHIFT:
	case N_GT:
	case N_GTE:
	case N_LT:
	case N_LTE:
	case N_EQ:
	case N_NEQ:
	case N_BITWISE_AND:
	case N_BITWISE_XOR:
	case N_BITWISE_OR:
	case N_LOGICAL_AND:
	case N_LOGICAL_OR:
	case N_ASSIGN:
	case N_ASSIGN_MULTIPLY:
	case N_ASSIGN_DIVIDE:
	case N_ASSIGN_MODULO:
	case N_ASSIGN_ADD:
	case N_ASSIGN_SUBTRACT:
	case N_ASSIGN_LSHIFT:
	case N_ASSIGN_RSHIFT:
	case N_ASSIGN_BITWISE_AND:
	case N_ASSIGN_BITWISE_XOR:
	case N_ASSIGN_BITWISE_OR:
		ast_print_node(node->binary_expression.lhs, depth, "", BRANCH);
		ast_print_node(node->binary_expression.rhs, depth, "", LEAF);
		break;
	default:
		break;
	}
}

void ast_print(struct ast_node *node) {
	ast_print_node(node, 0, "", "");
}

char *ast_nameof(enum ast_kind kind) {
	static char *names[AST_MAX] = {
		[N_INTEGER_LITERAL] = "literal",
		[N_IDENTIFIER] = "identifier",
		[N_ADD] = "+",
		[N_MULTIPLY] = "*",
		[N_CAST] = "cast",
	};
	return names[kind];
}

char *token_nameof(enum token_kind kind) {
	static char *names[TOKEN_MAX] = {
		[T_UNKNOWN] = "unknown",
		[T_EOF] = "eof",
		[T_WHITESPACE] = "whitespace",
		[T_NEWLINE] = "newline",
		[T_COMMENT] = "comment",
		[T_DOCUMENTATION] = "documentation",
		[TL_STRING] = "string literal",
		[TL_CHAR] = "char literal",
		[TL_CHARS] = "chars literal",
		[TL_FLOAT] = "float literal",
		[T_INTEGER_LITERAL] = "integer literal",
		[TM_UTF8] = "utf8",
		[TM_UTF16] = "utf16",
		[TM_UTF32] = "utf32",
		[TM_WIDE] = "wide",
		[TM_FLOAT] = "f",
		[TM_DOUBLE] = "d",
		[TM_LDOUBLE] = "l",
		[TM_DECIMAL32] = "df",
		[TM_DECIMAL64] = "dd",
		[TM_DECIMAL128] = "dl",
		[TM_UNSIGNED] = "u",
		[TM_LONG] = "l",
		[TM_LONGLONG] = "ll",
		[TM_BITINT] = "wb",
		[T_IDENTIFIER] = "identifier",
		[T_LPAREN] = "(",
		[T_RPAREN] = ")",
		[T_LBRACKET] = "[",
		[T_RBRACKET] = "]",
		[T_LCURLY] = "{",
		[T_RCURLY] = "}",
		[T_SEMICOLON] = ";",
		[T_COLON] = ":",
		[T_COMMA] = ",",
		[T_ADD] = "+",
		[T_INCREMENT] = "++",
		[T_SUBTRACT] = "-",
		[T_DECREMENT] = "--",
		[T_ASTERISK] = "*",
		[T_DIVIDE] = "/",
		[T_PERCENT] = "%",
		[T_TILDE] = "~",
		[T_EXCLAIM] = "!",
		[T_QUESTION] = "?",
		[T_EQ] = "==",
		[T_NEQ] = "!=",
		[T_GT] = ">",
		[T_GTE] = ">=",
		[T_LT] = "<",
		[T_LTE] = "<=",
		[T_AMPERSAND] = "&",
		[T_PIPE] = "|",
		[T_OR] = "||",
		[T_CARET] = "^",
		[T_AND] = "&&",
		[T_ACCESS] = ".",
		[T_INDIRECT_ACCESS] = "->",
		[T_LSHIFT] = "<<",
		[T_RSHIFT] = ">>",
		[T_ASSIGN] = "=",
		[T_ASSIGN_MULTIPLY] = "*=",
		[T_ASSIGN_DIVIDE] = "/=",
		[T_ASSIGN_MODULO] = "%=",
		[T_ASSIGN_ADD] = "+=",
		[T_ASSIGN_SUBTRACT] = "-=",
		[T_ASSIGN_LSHIFT] = "<<=",
		[T_ASSIGN_RSHIFT] = ">>=",
		[T_ASSIGN_BITWISE_AND] = "&=",
		[T_ASSIGN_BITWISE_XOR] = "^=",
		[T_ASSIGN_BITWISE_OR] = "|=",
		[T_ALIGNAS] = "alignas",
		[T_ALIGNOF] = "alignof",
		[T_AUTO] = "auto",
		[T_BITINT] = "_BitInt",
		[T_BOOL] = "bool",
		[T_CHAR] = "char",
		[T_COMPLEX] = "_Complex",
		[T_CONST] = "const",
		[T_DECIMAL32] = "_Decimal32",
		[T_DECIMAL64] = "_Decimal64",
		[T_DECIMAL128] = "_Decimal128",
		[T_DEFAULT] = "default",
		[T_DOUBLE] = "double",
		[T_ELSE] = "else",
		[T_FLOAT] = "float",
		[T_FOR] = "for",
		[T_GENERIC] = "_Generic",
		[T_IF] = "if",
		[T_INT] = "int",
		[T_LONG] = "long",
		[T_RETURN] = "return",
		[T_SHORT] = "short",
		[T_SIGNED] = "signed",
		[T_SIZEOF] = "sizeof",
		[T_UNSIGNED] = "unsigned",
		[T_VOID] = "void",
		[T_WHILE] = "while",
		[T_DIRECTIVE] = "#",
		[T_IFDEF] = "ifdef",
		[T_IFNDEF] = "ifndef",
		[T_ELIF] = "elif",
		[T_ELIFDEF] = "elifdef",
		[T_ELIFNDEF] = "elifndef",
		[T_ENDIF] = "endif",
		[T_INCLUDE] = "include",
		[T_EMBED] = "embed",
		[T_DEFINE] = "define",
		[T_UNDEF] = "undef",
		[T_LINE] = "line",
		[T_ERROR] = "error",
		[T_WARNING] = "warning",
		[T_PRAGMA] = "pragma",
	};
	return names[kind];
}
