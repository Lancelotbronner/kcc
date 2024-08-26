//
//  lexer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#include <kcc/lexer.h>

const char *BufferStart;
const char *BufferPtr;
const char *CurPtr;
#define Character (*CurPtr)
const char *BufferEnd;
#define IsAtEndOfFile (CurPtr == BufferEnd)

bool IsPreprocessorEnabled;
bool IsWhitespacePreserved;
bool IsCommentPreserved;
bool IsDocumentationPreserved;

bool IsInPreprocessorDirective;
