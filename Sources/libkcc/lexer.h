//
//  lexer.h
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#include <kcc/lexer.h>

//TODO: All lexer configuration and state should be in a struct that can be saved and restored or something

const char *BufferStart;
const char *BufferPtr;
const char *CurPtr;
char Character;
char PreviousCharacter;
const char *BufferEnd;
#define IsAtEndOfFile (CurPtr == BufferEnd)

char PutbackStack[8];
char Putback = 0;

bool IsPreprocessorEnabled;
bool IsWhitespacePreserved;
bool IsCommentPreserved;
bool IsDocumentationPreserved;
bool IsDigraphsEnabled;

bool HasLeadingWhitespace;
bool IsAtStartOfLine;
bool IsAtPhysicalStartOfLine;
bool IsInPreprocessorDirective;
