//
//  lexer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#include <kcc/lexer.h>

#include <kcc/scanner.h>

scanner_t Scanner;


bool IsPreprocessorEnabled;
bool IsWhitespacePreserved;
bool IsCommentPreserved;
bool IsDocumentationPreserved;
bool IsDigraphsEnabled;

bool HasLeadingWhitespace;
bool IsAtStartOfLine;
bool IsAtPhysicalStartOfLine;
bool IsInPreprocessorDirective;
