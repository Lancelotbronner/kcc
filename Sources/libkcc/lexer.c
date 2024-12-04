//
//  lexer.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#include "lexer.h"

#include "globals.h"
#include "unicode.h"

#include <assert.h>
#include <stdlib.h> // malloc

//FIXME: Workaround for parser
struct lexer __Lexer;
lexer_t Lexer = &__Lexer;

//MARK: - Lexer

struct lexer {
	//TODO: Copy scanner?
	scanner_t scanner;

	enum lexer_mode mode : 2;

	// Configuration flags
	bool is_whitespace_preserved : 1;
	bool is_digraphs_enabled : 1;

	// State flags
	bool is_at_start_of_line : 1;
	bool is_at_physical_start_of_line : 1;
};

#define IsCharacter(c1, c2) (scanner_peek(lexer->scanner) == (c1) || scanner_peek(lexer->scanner) == (c2))

size_t lexer_size() {
	return sizeof(struct lexer);
}

lexer_t lexer_alloc() {
	return malloc(sizeof(struct lexer));
}

void lexer_init(lexer_t lexer, scanner_t scanner) {
	*(lexer) = (struct lexer) {
		.scanner = scanner,
	};
}

enum lexer_mode lexer_mode(lexer_t lexer) {
	return lexer->mode;
}

void lexer_enter(lexer_t lexer, enum lexer_mode mode) {
	lexer->mode = mode;
}

bool lexer_eof(lexer_t lexer) {
	return scanner_eof(lexer->scanner);
}

bool lexer_match(lexer_t lexer, enum token_kind token) {
	if (Token.kind != token)
		return false;
	lexer_advance(lexer);
}

//MARK: - Token Utilities

static bool ProduceToken(lexer_t lexer, enum token_kind kind) {
	//TODO: Move Token, TokenLength and TokenSource into the lexer
	Token.kind = kind;
	TokenLength = scanner_length(lexer->scanner);
	TokenSource = scanner_token(lexer->scanner);
	scanner_anchor(lexer->scanner);
	return true;
}

static bool ConsumeToken(lexer_t lexer, enum token_kind kind) {
	scanner_advance(lexer->scanner);
	return ProduceToken(lexer, kind);
}

//MARK: - scanner_peek(lexer->scanner) Classes

static bool IsHorizontalWhitespace(char c) {
	switch (c) {
	case ' ': case '\t': case '\f': case '\v':
		return true;
	default:
		return false;
	}
}

static bool IsVerticalWhitespace(char c) {
	switch (c) {
	case '\r': case '\n':
		return true;
	default:
		return false;
	}
}

static bool IsBinaryDigit(char c) {
	switch (c) {
	case '\'':
	case '0': case '1':
		return true;
	default:
		return false;
	}
}

static bool IsOctalDigit(char c) {
	switch (c) {
	case '\'':
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
		return true;
	default:
		return false;
	}
}

static bool IsDecimalDigit(char c) {
	switch (c) {
	case '\'':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return true;
	default:
		return false;
	}
}

static bool IsHexadecimalDigit(char c) {
	switch (c) {
	case '\'':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return true;
	default:
		return false;
	}
}

//MARK: - Insignificant Tokens

static bool LexNull() {

}

static bool LexWhitespace() {

}

static bool LexLineComment() {
//	// Line comment.
//	// Even if Line comments are disabled (e.g. in C89 mode), we generally
//	// want to lex this as a comment.  There is one problem with this though,
//	// that in one particular corner case, this can change the behavior of the
//	// resultant program.  For example, In  "foo //**/ bar", C89 would lex
//	// this as "foo / bar" and languages with Line comments would lex it as
//	// "foo".  Check to see if the character after the second slash is a '*'.
//	// If so, we will lex that as a "/" instead of the start of a comment.
//	// However, we never do this if we are just preprocessing.
//	bool TreatAsComment =
//	LineComment && (LangOpts.CPlusPlus || !LangOpts.TraditionalCPP);
//	if (!TreatAsComment)
//		if (!(PP && PP->isPreprocessedOutput()))
//			TreatAsComment = getCharAndSize(CurPtr+SizeTmp, SizeTmp2) != '*';
//
//	if (TreatAsComment) {
//		if (SkipLineComment(Result, ConsumeChar(CurPtr, SizeTmp, Result),
//							TokAtPhysicalStartOfLine))
//			return true; // There is a token to return.
//
//		// It is common for the tokens immediately after a // comment to be
//		// whitespace (indentation for the next line).  Instead of going through
//		// the big switch, handle it efficiently now.
//		goto SkipIgnoredUnits;
//	}
}

static bool LexBlockComment() {
//	if (SkipBlockComment(Result, ConsumeChar(CurPtr, SizeTmp, Result),
//						 TokAtPhysicalStartOfLine))
//		return true; // There is a token to return.
//
//	// We only saw whitespace, so just try again with this lexer.
//	// (We manually eliminate the tail call to avoid recursion.)
//	goto lex_next;
}

/// LexEndOfFile - CurPtr points to the end of this file.  Handle this
/// condition, reporting diagnostics and handling other edge cases as required.
/// This returns true if Result contains a token, false if PP.Lex should be
/// called again.
static bool LexEndOfFile(lexer_t lexer) {
	// If we were in a preprocessor directive we should end it first.
	if (lexer->mode == LEXER_DIRECTIVE) {
		lexer->mode = LEXER_SOURCE;

		//TODO: Warn about missing newline in preprocessor directive in pedantic mode

		return ProduceToken(lexer, T_EOD);
	}

	//TODO: Handle End of Buffer vs End of File

	// Produce the End Of File token
	return ProduceToken(lexer, T_EOF);

//	if (PP->isRecordingPreamble() && PP->isInPrimaryFile()) {
//		PP->setRecordedPreambleConditionalStack(ConditionalStack);
//		// If the preamble cuts off the end of a header guard, consider it guarded.
//		// The guard is valid for the preamble content itself, and for tools the
//		// most useful answer is "yes, this file has a header guard".
//		if (!ConditionalStack.empty())
//			MIOpt.ExitTopLevelConditional();
//		ConditionalStack.clear();
//	}

	//TODO: Issue diagnostics for unterminated #if and missing newline.

	// If we are in a #if directive, emit an error.
//	while (!ConditionalStack.empty()) {
//		if (PP->getCodeCompletionFileLoc() != FileLoc)
//			PP->Diag(ConditionalStack.back().IfLoc,
//					 diag::err_pp_unterminated_conditional);
//		ConditionalStack.pop_back();
//	}

	//TODO: Handle pedantic C newline
	// C99 5.1.1.2p2: If the file is non-empty and didn't end in a newline, issue
	// a pedwarn.
//	if (CurPtr != BufferStart && (CurPtr[-1] != '\n' && CurPtr[-1] != '\r')) {
//		DiagnosticsEngine &Diags = PP->getDiagnostics();
//		SourceLocation EndLoc = getSourceLocation(BufferEnd);
//		unsigned DiagID;
//
//		if (LangOpts.CPlusPlus11) {
//			// C++11 [lex.phases] 2.2 p2
//			// Prefer the C++98 pedantic compatibility warning over the generic,
//			// non-extension, user-requested "missing newline at EOF" warning.
//			if (!Diags.isIgnored(diag::warn_cxx98_compat_no_newline_eof, EndLoc)) {
//				DiagID = diag::warn_cxx98_compat_no_newline_eof;
//			} else {
//				DiagID = diag::warn_no_newline_eof;
//			}
//		} else {
//			DiagID = diag::ext_no_newline_eof;
//		}
//
//		Diag(BufferEnd, DiagID)
//		<< FixItHint::CreateInsertion(EndLoc, "\n");
//	}
}

//MARK: - Numeric Literals

static void TryConsumeInvalidDigitSeparator(lexer_t lexer) {
	if (scanner_peek(lexer->scanner) != '\'')
		return;
	//TODO: Diagnose illegal digit separator
}

static void LexNumericConstantPrefix(lexer_t lexer) {
	assert(scanner_peek(lexer->scanner) == '0');
	scanner_advance(lexer->scanner);

	switch (scanner_peek(lexer->scanner)) {
	case 'x': case 'X':
		Token.modifiers.base = BASE_HEX;
		scanner_advance(lexer->scanner);
		return;
	case 'b': case 'B':
		Token.modifiers.base = BASE_BINARY;
		scanner_advance(lexer->scanner);
		return;
	case '8': case '9':
		Token.modifiers.base = BASE_DECIMAL;
		return;
	default:
		Token.modifiers.base = BASE_OCTAL;
		return;
	}
}

static bool LexIntegerSuffix(lexer_t lexer) {
	Token.modifiers.integer.suffix = ISUFFIX_NONE;
	Token.modifiers.integer.is_unsigned = false;

	if (IsCharacter('u', 'U'))
		Token.modifiers.integer.is_unsigned = true;

	switch (scanner_peek(lexer->scanner)) {
	case 'l': case 'L':
		scanner_advance(lexer->scanner);

		if (!IsCharacter('l', 'L')) {
			Token.modifiers.integer.suffix = ISUFFIX_LONG;
			break;
		}

		scanner_advance(lexer->scanner);
		Token.modifiers.integer.suffix = ISUFFIX_LONGLONG;
		break;

	case 'w': case 'W':
		scanner_advance(lexer->scanner);

		if (!IsCharacter('b', 'B')) {
			scanner_putback(lexer->scanner, scanner_peek(lexer->scanner));
			break;
		}

		scanner_advance(lexer->scanner);
		Token.modifiers.integer.suffix = ISUFFIX_BITINT;
		break;
	}

	if (!Token.modifiers.integer.is_unsigned && IsCharacter('u', 'U'))
		Token.modifiers.integer.is_unsigned = true;

	return ProduceToken(lexer, T_INTEGER_LITERAL);
}

static bool LexFloatingSuffix(lexer_t lexer) {
	Token.modifiers.floating.suffix = FSUFFIX_NONE;
	switch (scanner_peek(lexer->scanner)) {
	case 'f': case 'F':
		Token.modifiers.floating.suffix = FSUFFIX_FLOAT;
		break;
	case 'l': case 'L':
		Token.modifiers.floating.suffix = FSUFFIX_LONGDOUBLE;
		break;
	case 'd': case 'D':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case 'f': case 'F':
			Token.modifiers.floating.suffix = FSUFFIX_DECIMAL32;
			break;
		case 'd': case 'D':
			Token.modifiers.floating.suffix = FSUFFIX_DECIMAL64;
			break;
		case 'l': case 'L':
			Token.modifiers.floating.suffix = FSUFFIX_DECIMAL128;
			break;
		default:
			scanner_putback(lexer->scanner, scanner_peek(lexer->scanner));
			break;
		}
		scanner_advance(lexer->scanner);
		break;
	}
}

static void LexExponentPart(lexer_t lexer) {
	if (IsCharacter('+', '-'))
		scanner_advance(lexer->scanner);

	while (IsDecimalDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);
}

static bool LexDecimalFractionalPart(lexer_t lexer) {
	assert(IsDecimalDigit(scanner_peek(lexer->scanner)));
	TryConsumeInvalidDigitSeparator(lexer);

	while (IsHexadecimalDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);

	switch (scanner_peek(lexer->scanner)) {
	case 'E': case 'e':
		scanner_advance(lexer->scanner);
		LexExponentPart(lexer);
		return LexFloatingSuffix(lexer);

	case 'P': case 'p':
		//TODO: Potentially diagnose invalid exponent marker?
		[[fallthrough]];
	default:
		return LexIntegerSuffix(lexer);
	}
}

static bool LexHexadecimalFractionalPart(lexer_t lexer) {
	assert(IsHexadecimalDigit(scanner_peek(lexer->scanner)));
	TryConsumeInvalidDigitSeparator(lexer);

	switch (scanner_peek(lexer->scanner)) {
	case 'P': case 'p':
		scanner_advance(lexer->scanner);
		LexExponentPart(lexer);
		// C (6.4.4.2 p2):
		// A floating suffix shall not be used in a hexadecimal floating constant.
		break;

	case 'E': case 'e':
		//TODO: Potentially diagnose invalid exponent marker?
	}

	return ProduceToken(lexer, T_FLOATING_LITERAL);
}

static bool LexBinaryConstant(lexer_t lexer) {
	while (IsBinaryDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);
	return LexIntegerSuffix(lexer);
}

static bool LexOctalConstant(lexer_t lexer) {
	while (IsOctalDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);
	return LexIntegerSuffix(lexer);
}

static bool LexDecimalConstant(lexer_t lexer) {
	while (IsDecimalDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);

	switch (scanner_peek(lexer->scanner)) {
	case '.':
		scanner_advance(lexer->scanner);
		return LexDecimalFractionalPart(lexer);

	case 'E': case 'e':
		scanner_advance(lexer->scanner);
		LexExponentPart(lexer);
		return LexFloatingSuffix(lexer);

	case 'P': case 'p':
		//TODO: Potentially diagnose invalid exponent marker?
		[[fallthrough]];
	default:
		return LexIntegerSuffix(lexer);
	}
}

static bool LexHexadecimalConstant(lexer_t lexer) {
	while (IsHexadecimalDigit(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);

	switch (scanner_peek(lexer->scanner)) {
	case '.':
		scanner_advance(lexer->scanner);
		return LexHexadecimalFractionalPart(lexer);

	case 'P': case 'p':
		scanner_advance(lexer->scanner);
		LexExponentPart(lexer);
		// C (6.4.4.2 p2):
		// A floating suffix shall not be used in a hexadecimal floating constant.
		return ProduceToken(lexer, T_FLOATING_LITERAL);

	case 'E': case 'e':
		//TODO: Potentially diagnose invalid exponent marker?
		[[fallthrough]];
	default:
		return LexIntegerSuffix(lexer);
	}
}

static bool LexNumericConstant(lexer_t lexer) {
	assert(IsHexadecimalDigit(scanner_peek(lexer->scanner)));
	TryConsumeInvalidDigitSeparator(lexer);

	switch (Token.modifiers.base) {
	case BASE_BINARY: return LexBinaryConstant(lexer);
	case BASE_OCTAL: return LexOctalConstant(lexer);
	case BASE_DECIMAL: return LexDecimalConstant(lexer);
	case BASE_HEX: return LexHexadecimalConstant(lexer);
	case BASE_NONE: return LexDecimalConstant(lexer);
	default: assert(false);
	}
}

//MARK: - String & Character Literals

/// LexStringLiteral - Lex the remainder of a string literal, after having lexed
/// either " or L" or u8" or u" or U".
static bool LexStringLiteral(lexer_t lexer) {
	//TODO: Compare this with Clang source
	scanner_advance(lexer->scanner);

	while (scanner_peek(lexer->scanner) != '"') {
		// Skip escaped characters.
		// Escaped newlines will already be processed by scanner_advance.
		if (scanner_peek(lexer->scanner) == '\\')
			scanner_advance(lexer->scanner);

		// Detect and report unterminated string literals.
		if (scanner_eof(lexer->scanner)) {
			//TODO: Diagnose unterminated string literal
			scanner_anchor(lexer->scanner);
			return true;
		}

		switch (scanner_peek(lexer->scanner)) {
		case '\n':
		case '\r':
			scanner_anchor(lexer->scanner);
			return true;
		}

		if (scanner_peek(lexer->scanner) == '\0') {
			//TODO: Diagnose null character.
		}

		scanner_advance(lexer->scanner);
	}

	return ProduceToken(lexer, T_STRING_LITERAL);
}

/// LexCharConstant - Lex the remainder of a character constant, after having
/// lexed either ' or L' or u8' or u' or U'.
static bool LexCharConstant() {
	// Does this character contain the \0 character?
//	const char *NulCharacter = nullptr;
//
//	if (!isLexingRawMode()) {
//		if (Kind == tok::utf16_char_constant || Kind == tok::utf32_char_constant)
//			Diag(BufferPtr, LangOpts.CPlusPlus
//				 ? diag::warn_cxx98_compat_unicode_literal
//				 : diag::warn_c99_compat_unicode_literal);
//		else if (Kind == tok::utf8_char_constant)
//			Diag(BufferPtr, diag::warn_cxx14_compat_u8_character_literal);
//	}
//
//	char C = getAndAdvanceChar(CurPtr, Result);
//	if (C == '\'') {
//		if (!isLexingRawMode() && !LangOpts.AsmPreprocessor)
//			Diag(BufferPtr, diag::ext_empty_character);
//		FormTokenWithChars(Result, CurPtr, tok::unknown);
//		return true;
//	}
//
//	while (C != '\'') {
//		// Skip escaped characters.
//		if (C == '\\')
//			C = getAndAdvanceChar(CurPtr, Result);
//
//		if (C == '\n' || C == '\r' ||             // Newline.
//			(C == 0 && CurPtr-1 == BufferEnd)) {  // End of file.
//			if (!isLexingRawMode() && !LangOpts.AsmPreprocessor)
//				Diag(BufferPtr, diag::ext_unterminated_char_or_string) << 0;
//			FormTokenWithChars(Result, CurPtr-1, tok::unknown);
//			return true;
//		}
//
//		if (C == 0) {
//			if (isCodeCompletionPoint(CurPtr-1)) {
//				PP->CodeCompleteNaturalLanguage();
//				FormTokenWithChars(Result, CurPtr-1, tok::unknown);
//				cutOffLexing();
//				return true;
//			}
//
//			NulCharacter = CurPtr-1;
//		}
//		C = getAndAdvanceChar(CurPtr, Result);
//	}
//
//	// If we are in C++11, lex the optional ud-suffix.
//	if (LangOpts.CPlusPlus)
//		CurPtr = LexUDSuffix(Result, CurPtr, false);
//
//	// If a nul character existed in the character, warn about it.
//	if (NulCharacter && !isLexingRawMode())
//		Diag(NulCharacter, diag::null_in_char_or_string) << 0;
//
//	// Update the location of token as well as BufferPtr.
//	const char *TokStart = BufferPtr;
//	FormTokenWithChars(Result, CurPtr, Kind);
//	Result.setLiteralData(TokStart);
//	return true;
}

//MARK: - Significant Tokens

static bool LexIdentifier(lexer_t lexer) {
	while (isxidcontinue(scanner_peek(lexer->scanner)))
		scanner_advance(lexer->scanner);

	// Identify the keyword or return an identifier
	//TODO: While parsing attributes always return T_IDENTIFIER
	ptrdiff_t length = scanner_length(lexer->scanner);
	char const *token = scanner_token(lexer->scanner);
	enum token_kind kind = (lexer->mode == LEXER_DIRECTIVE ? token_preprocessor : token_keyword)(token, length);

	return ProduceToken(lexer, kind);
}

static bool LexDot(lexer_t lexer) {
	// Consume the initial period.
	scanner_advance(lexer->scanner);

	// Parse floating constants of the form `.25`.
	if (scanner_peek(lexer->scanner) >= '0' && scanner_peek(lexer->scanner) <= '9')
		return LexNumericConstant(lexer);

	// If we're not parsing an ellipsis then its a period.
	if (scanner_peek(lexer->scanner) != '.')
		return ProduceToken(lexer, T_PERIOD);

	// Consume the second period.
	scanner_advance(lexer->scanner);

	// If we have a third period its an elipsis.
	if (scanner_peek(lexer->scanner) == '.')
		return ConsumeToken(lexer, T_ELLIPSIS);

	// It wasn't an ellipsis.
	Token.kind = T_PERIOD;
	scanner_putback(lexer->scanner, scanner_peek(lexer->scanner));
}

static bool LexDirective() {

}

static bool LexAngledStringLiteral() {

}

//MARK: - Lexer

static bool _lexer_advance_preprocessor(lexer_t lexer) {
lex_start:
	switch (scanner_peek(lexer->scanner)) {
	case '\0':
		if (scanner_eof(lexer->scanner)) {
			if (Token.kind != T_EOF)
				ProduceToken(lexer, T_EOF);
			return false;
		}

		//TODO: Start diagnosing unexpected null in file
		Token.flags.leading_space = true;

		//TODO: Special case skip if we have a bunch of sequential nulls?
		// Take into account end-of-buffer
		scanner_advance(lexer->scanner);

		// Try again, manually eliminate the tail call to avoid recursion.
		goto lex_next;

	case '\r':
		scanner_consume(lexer->scanner, '\n');
		[[fallthrough]];
	case '\n':
		// Signals the end of a preprocessor directive.
		if (lexer->mode == LEXER_DIRECTIVE) {
			lexer->mode = LEXER_SOURCE;

			// Since we consumed a newline, we are back at the start of a line.
			lexer->is_at_start_of_line = true;
			lexer->is_at_physical_start_of_line = true;

			return ProduceToken(lexer, T_EOD);
		}

		// No leading whitespace seen so far.
		Token.flags.leading_space = false;

		if (lexer->is_whitespace_preserved)
			return LexWhitespace();

		// We only saw whitespace, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;
	case ' ':
	case '\t':
	case '\f':
	case '\v':
		Token.flags.leading_space = true;
		do scanner_advance(lexer->scanner);
		while (IsHorizontalWhitespace(scanner_peek(lexer->scanner)));

		if (lexer->is_whitespace_preserved)
			return ProduceToken(lexer, T_WHITESPACE);

		// We only saw whitespace, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;

		// C99 6.4.4.1: Integer Constants.
		// C99 6.4.4.2: Floating Constants.
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return LexNumericConstant(lexer);

		// Identifier
		// Wide literal
	case 'L':
		scanner_advance(lexer->scanner);
		Token.modifiers.encoding = ENCODING_WIDE;

		switch (scanner_peek(lexer->scanner)) {
		case '"': return LexStringLiteral(lexer);
		case '\'': return LexCharConstant();
		default: return LexIdentifier(lexer);
		}

		// Identifier
		// C23/C++17: UTF-8 literal
		// C11/C++11: UTF-16 literal
	case 'u':
		scanner_advance(lexer->scanner);
		Token.modifiers.encoding = scanner_consume(lexer->scanner, '8') ? ENCODING_UTF8 : ENCODING_UTF16;

		switch (scanner_peek(lexer->scanner)) {
		case '"': return LexStringLiteral(lexer);
		case '\'': return LexCharConstant();
		default: return LexIdentifier(lexer);
		}

		// Identifier
		// C11/C++11: UTF-32 literal
	case 'U':
		scanner_advance(lexer->scanner);
		Token.modifiers.encoding = ENCODING_UTF32;

		switch (scanner_peek(lexer->scanner)) {
		case '"': return LexStringLiteral(lexer);
		case '\'': return LexCharConstant();
		default: return LexIdentifier(lexer);
		}

		// C99 6.4.2: Identifiers.
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': /* 'L' */ case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': /* 'U' */
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': /* 'u' */
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		scanner_advance(lexer->scanner);
		return LexIdentifier(lexer);

		// C99 6.4.4: scanner_peek(lexer->scanner) Constants.
	case '\'':
		Token.modifiers.encoding = ENCODING_NONE;
		return LexCharConstant();

		// C99 6.4.5: String Literals.
	case '"':
		Token.modifiers.encoding = ENCODING_NONE;
		return LexStringLiteral(lexer);

		// C99 6.4.6: Punctuators.
	case '?':
		return ConsumeToken(lexer, T_QUESTION);
	case '[':
		return ConsumeToken(lexer, T_LBRACKET);
	case ']':
		return ConsumeToken(lexer, T_RBRACKET);
	case '(':
		return ConsumeToken(lexer, T_LPAREN);
	case ')':
		return ConsumeToken(lexer, T_RPAREN);
	case '{':
		return ConsumeToken(lexer, T_LCURLY);
	case '}':
		return ConsumeToken(lexer, T_RCURLY);

		// Floating-point constant
		// Operators . ...
	case '.':
		if (scanner_peek(lexer->scanner) >= 0 && scanner_peek(lexer->scanner) <= '9')
			return LexNumericConstant(lexer);

		scanner_advance(lexer->scanner);

		//TODO: Ellipsis operator
		switch (scanner_peek(lexer->scanner)) {
		default: return ProduceToken(lexer, T_PERIOD);
		}

		// Operators & && &=
	case '&':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '&': return ConsumeToken(lexer, T_LOGICAL_AND);
		case '=': return ConsumeToken(lexer, T_ASSIGN_BITWISE_AND);
		default: return ProduceToken(lexer, T_AMPERSAND);
		}

		// Operators * *=
	case '*':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_ASSIGN_MULTIPLY);
		default: return ProduceToken(lexer, T_ASTERISK);
		}

		// Operators + ++ +=
	case '+':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '+': return ConsumeToken(lexer, T_INCREMENT);
		case '=': return ConsumeToken(lexer, T_ASSIGN_ADD);
		default: return ProduceToken(lexer, T_PLUS);
		}

		// Operators - -- -> -=
	case '-':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '-': return ConsumeToken(lexer, T_DECREMENT);
		case '=': return ConsumeToken(lexer, T_ASSIGN_SUBTRACT);
		case '>':
			scanner_advance(lexer->scanner);
			switch (scanner_peek(lexer->scanner)) {
			case '*': return ConsumeToken(lexer, T_ARROW_STAR);
			default: return ProduceToken(lexer, T_ARROW);
			}
		default: return ProduceToken(lexer, T_SUBTRACT);
		}

		// Operators ~
	case '~':
		return ConsumeToken(lexer, T_TILDE);

		// Operators ! !=
	case '!':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_NEQ);
		default: return ProduceToken(lexer, T_EXCLAIM);
		}

		// 6.4.9: Comments
		// Operators / /=
	case '/':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '/': return LexLineComment();
		case '*': return LexBlockComment();
		case '=': return ConsumeToken(lexer, T_ASSIGN_DIVIDE);
		default: return ProduceToken(lexer, T_DIVIDE);
		}

		// Operators % %=
		// Digraphs %> %: %:%:
	case '%':
		scanner_advance(lexer->scanner);

		if (lexer->is_digraphs_enabled) {
			switch (scanner_peek(lexer->scanner)) {
			case '>': return ConsumeToken(lexer, T_RCURLY);
			case ':':
				scanner_advance(lexer->scanner);
				switch (scanner_peek(lexer->scanner)) {
				case '%':
					scanner_advance(lexer->scanner);
					switch (scanner_peek(lexer->scanner)) {
					case ':': return ConsumeToken(lexer, T_STRINGIFY);
					default:
						scanner_putback(lexer->scanner, '%');
						scanner_putback(lexer->scanner, scanner_peek(lexer->scanner));
						return ProduceToken(lexer, T_DIRECTIVE);
					}
				default: return ProduceToken(lexer, T_DIRECTIVE);
				}
			}
		}

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_ASSIGN_MODULO);
		default: return ProduceToken(lexer, T_PERCENT);
		}

		// Angled string literal
		// Operators < <= << <<=
		// Digraphs <: <%
	case '<':
		if (lexer->is_digraphs_enabled) {
			//TODO: Parse digraphs
		}
		if (lexer->mode == LEXER_DIRECTIVE)
			return LexAngledStringLiteral();

		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_LTE);
		case '<':
			scanner_advance(lexer->scanner);
			switch (scanner_peek(lexer->scanner)) {
				//TODO: Detect C++ spaceship operator
			case '=': return ConsumeToken(lexer, T_ASSIGN_LSHIFT);
			default: return ProduceToken(lexer, T_LSHIFT);
			}
		default: return ProduceToken(lexer, T_LT);
		}

		// Operators > >= >> >>=
	case '>':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '>':
			scanner_advance(lexer->scanner);
			switch (scanner_peek(lexer->scanner)) {
			case '=': return ConsumeToken(lexer, T_ASSIGN_RSHIFT);
			default: return ProduceToken(lexer, T_RSHIFT);
			}
		case '=': return ConsumeToken(lexer, T_GTE);
		default: return ProduceToken(lexer, T_GT);
		}

		// Operators ^ ^=
	case '^':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_ASSIGN_BITWISE_XOR);
		default: return ProduceToken(lexer, T_CARET);
		}

		// Operators | || |=
	case '|':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_ASSIGN_BITWISE_OR);
		case '|': return ConsumeToken(lexer, T_LOGICAL_OR);
		default: return ProduceToken(lexer, T_PIPE);
		}

		// Operators : ::
		// Digraphs :>
	case ':':
		scanner_advance(lexer->scanner);

		//TODO: Handle digraphs

		switch (scanner_peek(lexer->scanner)) {
		case ':': return ConsumeToken(lexer, T_MODULE);
		case '|': return ConsumeToken(lexer, T_LOGICAL_OR);
		default: return ProduceToken(lexer, T_PIPE);
		}

	case ';':
		return ConsumeToken(lexer, T_SEMICOLON);

		// Operators = ==
	case '=':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '=': return ConsumeToken(lexer, T_EQ);
		default: return ProduceToken(lexer, T_ASSIGN);
		}

		// Operators ,
	case ',':
		return ConsumeToken(lexer, T_COMMA);

		// Preprocessor # ##
	case '#':
		scanner_advance(lexer->scanner);

		switch (scanner_peek(lexer->scanner)) {
		case '#': return ConsumeToken(lexer, T_STRINGIFY);
		default:
			if (lexer->is_at_physical_start_of_line) {
				lexer->mode = LEXER_DIRECTIVE;
				return ProduceToken(lexer, T_DIRECTIVE);
			}
			return ProduceToken(lexer, T_POUND);
		}

		//TODO: Detect preprocessor directives
		// We parsed a # character.  If this occurs at the start of the line,
		// it's actually the start of a preprocessing directive.  Callback to
		// the preprocessor to handle it.
		// TODO: -fpreprocessed mode??
//		if (TokAtPhysicalStartOfLine && !LexingRawMode && !Is_PragmaLexer)
//			goto lex_directive;

		// C99 6.4.3:
		// C++11 [lex.charset]p2:
		//   UCNs
	case '\\':
//		if (uint32_t CodePoint = tryReadUCN(CurPtr, BufferPtr, &Result)) {
//			if (CheckUnicodeWhitespace(Result, CodePoint, CurPtr)) {
//				if (SkipWhitespace(Result, CurPtr, TokAtPhysicalStartOfLine))
//					return true; // KeepWhitespaceMode
//
//				// We only saw whitespace, so just try again with this lexer.
//				// (We manually eliminate the tail call to avoid recursion.)
//				goto lex_next;
//			}
//
//			return LexUnicodeIdentifierStart(Result, CodePoint, CurPtr);
//		}
//
//		Kind = tok::unknown;
		break;

	default:
		if (isxidstart(scanner_peek(lexer->scanner)))
			return LexIdentifier(lexer);

//		if (isLexingRawMode() || ParsingPreprocessorDirective ||
//			PP->isPreprocessedOutput()) {
//			++CurPtr;
//			Kind = tok::unknown;
//			break;
//		}

		// Non-ASCII characters tend to creep into source code unintentionally.
		// Instead of letting the parser complain about the unknown token,
		// just diagnose the invalid UTF-8, then drop the character.
//		Diag(CurPtr, diag::err_invalid_utf8);

		scanner_advance(lexer->scanner);
		// We're pretending the character didn't exist, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;
	}

	// Update the location of token as well as BufferPtr.
	scanner_anchor(lexer->scanner);
	return true;

lex_next:
//	Result.clearFlag(Token::NeedsCleaning);
	scanner_anchor(lexer->scanner);
	goto lex_start;
}

bool lexer_advance(lexer_t lexer) {
	//TODO: Flag first token of line?
	//TODO: Flag whether token has leading whitespace?
	//TODO: Flag whether token is after empty macro?


	//TODO: Detect physical start of line?

	return _lexer_advance_preprocessor(lexer);
}
