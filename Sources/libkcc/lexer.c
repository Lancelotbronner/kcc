//
//  lexer.c
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-17.
//

#include "lexer.h"

#include "globals.h"
#include "unicode.h"

//MARK: - Scanner

static void scanner_anchor() {
	TokenLength = CurPtr - BufferPtr;
	Column += TokenLength;

	//TODO: Record current location?
	BufferPtr = CurPtr;
}

/// Advances to the next character, taking newline breaks into account.
///
/// - See Also: `5.1.1.2 Translation phases`
static void scanner_advance() {
	//TODO: Double-check this backslash business
	bool isBackslash = Character == '\\';
	CurPtr++;
	if (!isBackslash)
		return;

	if (Character == '\r')
		CurPtr++;
	if (Character != '\n')
		return;
	CurPtr++;
	Line++;
	Column = 1;
}

static bool scanner_consume(char character) {
	bool match = Character == character;
	if (match)
		scanner_advance();
	return match;
}

static void scanner_putback(char c) {
	//TODO: Figure out Putback
}

//MARK: - Lexing Utilities

static bool ProduceToken(enum token_kind kind) {
	Token.kind = kind;
	scanner_anchor();
	return true;
}

static bool ConsumeToken(enum token_kind kind) {
	scanner_advance();
	return ProduceToken(kind);
}

//MARK: - Lexer

static bool IsHorizontalWhitespace(char c) {
	switch (c) {
	case ' ':
	case '\t':
	case '\f':
	case '\v':
		return true;
	default:
		return false;
	}
}

static bool IsVerticalWhitespace(char c) {
	switch (c) {
	case '\r':
	case '\n':
		return true;
	default:
		return false;
	}
}

/// LexEndOfFile - CurPtr points to the end of this file.  Handle this
/// condition, reporting diagnostics and handling other edge cases as required.
/// This returns true if Result contains a token, false if PP.Lex should be
/// called again.
static bool LexEndOfFile() {
	// If we hit the end of the file while parsing a preprocessor directive,
	// end the preprocessor directive first.  The next token returned will
	// then be the end of file.
	if (ParsingPreprocessorDirective) {
		// Done parsing the "line".
		ParsingPreprocessorDirective = false;
		// Update the location of token as well as BufferPtr.
		FormTokenWithChars(Result, CurPtr, tok::eod);

		// Restore comment saving mode, in case it was disabled for directive.
		if (PP)
			resetExtendedTokenMode();
		return true;  // Have a token.
	}

	// If we are in raw mode, return this event as an EOF token.  Let the caller
	// that put us in raw mode handle the event.
	if (isLexingRawMode()) {
		Result.startToken();
		BufferPtr = BufferEnd;
		FormTokenWithChars(Result, BufferEnd, tok::eof);
		return true;
	}

	if (PP->isRecordingPreamble() && PP->isInPrimaryFile()) {
		PP->setRecordedPreambleConditionalStack(ConditionalStack);
		// If the preamble cuts off the end of a header guard, consider it guarded.
		// The guard is valid for the preamble content itself, and for tools the
		// most useful answer is "yes, this file has a header guard".
		if (!ConditionalStack.empty())
			MIOpt.ExitTopLevelConditional();
		ConditionalStack.clear();
	}

	// Issue diagnostics for unterminated #if and missing newline.

	// If we are in a #if directive, emit an error.
	while (!ConditionalStack.empty()) {
		if (PP->getCodeCompletionFileLoc() != FileLoc)
			PP->Diag(ConditionalStack.back().IfLoc,
					 diag::err_pp_unterminated_conditional);
		ConditionalStack.pop_back();
	}

	// C99 5.1.1.2p2: If the file is non-empty and didn't end in a newline, issue
	// a pedwarn.
	if (CurPtr != BufferStart && (CurPtr[-1] != '\n' && CurPtr[-1] != '\r')) {
		DiagnosticsEngine &Diags = PP->getDiagnostics();
		SourceLocation EndLoc = getSourceLocation(BufferEnd);
		unsigned DiagID;

		if (LangOpts.CPlusPlus11) {
			// C++11 [lex.phases] 2.2 p2
			// Prefer the C++98 pedantic compatibility warning over the generic,
			// non-extension, user-requested "missing newline at EOF" warning.
			if (!Diags.isIgnored(diag::warn_cxx98_compat_no_newline_eof, EndLoc)) {
				DiagID = diag::warn_cxx98_compat_no_newline_eof;
			} else {
				DiagID = diag::warn_no_newline_eof;
			}
		} else {
			DiagID = diag::ext_no_newline_eof;
		}

		Diag(BufferEnd, DiagID)
		<< FixItHint::CreateInsertion(EndLoc, "\n");
	}

	BufferPtr = CurPtr;

	// Finally, let the preprocessor handle this.
	return PP->HandleEndOfFile(Result, isPragmaLexer());
}

static bool LexIdentifierContinue() {
	scanner_advance();
	while (isxidcontinue(Character))
		CurPtr++;
	return ProduceToken(T_IDENTIFIER);
}

/// LexNumericConstant - Lex the remainder of a integer or floating point
/// constant. From[-1] is the first character lexed.  Return the end of the
/// constant.
static bool LexNumericConstant() {
	unsigned Size;
	char C = getCharAndSize(CurPtr, Size);
	char PrevCh = 0;
	while (isPreprocessingNumberBody(C)) {
		CurPtr = ConsumeChar(CurPtr, Size, Result);
		PrevCh = C;
		if (LangOpts.HLSL && C == '.' && (*CurPtr == 'x' || *CurPtr == 'r')) {
			CurPtr -= Size;
			break;
		}
		C = getCharAndSize(CurPtr, Size);
	}

	// If we fell out, check for a sign, due to 1e+12.  If we have one, continue.
	if ((C == '-' || C == '+') && (PrevCh == 'E' || PrevCh == 'e')) {
		// If we are in Microsoft mode, don't continue if the constant is hex.
		// For example, MSVC will accept the following as 3 tokens: 0x1234567e+1
		if (!LangOpts.MicrosoftExt || !isHexaLiteral(BufferPtr, LangOpts))
			return LexNumericConstant(Result, ConsumeChar(CurPtr, Size, Result));
	}

	// If we have a hex FP constant, continue.
	if ((C == '-' || C == '+') && (PrevCh == 'P' || PrevCh == 'p')) {
		// Outside C99 and C++17, we accept hexadecimal floating point numbers as a
		// not-quite-conforming extension. Only do so if this looks like it's
		// actually meant to be a hexfloat, and not if it has a ud-suffix.
		bool IsHexFloat = true;
		if (!LangOpts.C99) {
			if (!isHexaLiteral(BufferPtr, LangOpts))
				IsHexFloat = false;
			else if (!LangOpts.CPlusPlus17 &&
					 std::find(BufferPtr, CurPtr, '_') != CurPtr)
				IsHexFloat = false;
		}
		if (IsHexFloat)
			return LexNumericConstant(Result, ConsumeChar(CurPtr, Size, Result));
	}

	// If we have a digit separator, continue.
	if (C == '\'' && (LangOpts.CPlusPlus14 || LangOpts.C23)) {
		auto [Next, NextSize] = getCharAndSizeNoWarn(CurPtr + Size, LangOpts);
		if (isAsciiIdentifierContinue(Next)) {
			if (!isLexingRawMode())
				Diag(CurPtr, LangOpts.CPlusPlus
					 ? diag::warn_cxx11_compat_digit_separator
					 : diag::warn_c23_compat_digit_separator);
			CurPtr = ConsumeChar(CurPtr, Size, Result);
			CurPtr = ConsumeChar(CurPtr, NextSize, Result);
			return LexNumericConstant(Result, CurPtr);
		}
	}

	// If we have a UCN or UTF-8 character (perhaps in a ud-suffix), continue.
	if (C == '\\' && tryConsumeIdentifierUCN(CurPtr, Size, Result))
		return LexNumericConstant(Result, CurPtr);
	if (!isASCII(C) && tryConsumeIdentifierUTF8Char(CurPtr, Result))
		return LexNumericConstant(Result, CurPtr);

	// Update the location of token as well as BufferPtr.
	const char *TokStart = BufferPtr;
	FormTokenWithChars(Result, CurPtr, tok::numeric_constant);
	Result.setLiteralData(TokStart);
	return true;
}

/// LexStringLiteral - Lex the remainder of a string literal, after having lexed
/// either " or L" or u8" or u" or U".
static bool LexStringLiteral() {
	//TODO: Compare this with Clang source
	StringLiteral = CurPtr;
	scanner_advance();

	while (Character != '"') {
		// Skip escaped characters.
		// Escaped newlines will already be processed by scanner_advance.
		if (Character == '\\')
			scanner_advance();

		// Detect and report unterminated string literals.
		if (IsAtEndOfFile) {
			//TODO: Diagnose unterminated string literal
			scanner_anchor();
			return true;
		}

		switch (Character) {
		case '\n':
		case '\r':
			scanner_anchor();
			return true;
		}

		if (Character == '\0') {
			//TODO: Diagnose null character.
		}

		scanner_advance();
	}


	// Update the location of the token as well as the BufferPtr instance var.
	const char *TokStart = BufferPtr;
	FormTokenWithChars(Result, CurPtr, Kind);
	Result.setLiteralData(TokStart);
	return true;
}

/// LexCharConstant - Lex the remainder of a character constant, after having
/// lexed either ' or L' or u8' or u' or U'.
static bool LexCharConstant() {
	// Does this character contain the \0 character?
	const char *NulCharacter = nullptr;

	if (!isLexingRawMode()) {
		if (Kind == tok::utf16_char_constant || Kind == tok::utf32_char_constant)
			Diag(BufferPtr, LangOpts.CPlusPlus
				 ? diag::warn_cxx98_compat_unicode_literal
				 : diag::warn_c99_compat_unicode_literal);
		else if (Kind == tok::utf8_char_constant)
			Diag(BufferPtr, diag::warn_cxx14_compat_u8_character_literal);
	}

	char C = getAndAdvanceChar(CurPtr, Result);
	if (C == '\'') {
		if (!isLexingRawMode() && !LangOpts.AsmPreprocessor)
			Diag(BufferPtr, diag::ext_empty_character);
		FormTokenWithChars(Result, CurPtr, tok::unknown);
		return true;
	}

	while (C != '\'') {
		// Skip escaped characters.
		if (C == '\\')
			C = getAndAdvanceChar(CurPtr, Result);

		if (C == '\n' || C == '\r' ||             // Newline.
			(C == 0 && CurPtr-1 == BufferEnd)) {  // End of file.
			if (!isLexingRawMode() && !LangOpts.AsmPreprocessor)
				Diag(BufferPtr, diag::ext_unterminated_char_or_string) << 0;
			FormTokenWithChars(Result, CurPtr-1, tok::unknown);
			return true;
		}

		if (C == 0) {
			if (isCodeCompletionPoint(CurPtr-1)) {
				PP->CodeCompleteNaturalLanguage();
				FormTokenWithChars(Result, CurPtr-1, tok::unknown);
				cutOffLexing();
				return true;
			}

			NulCharacter = CurPtr-1;
		}
		C = getAndAdvanceChar(CurPtr, Result);
	}

	// If we are in C++11, lex the optional ud-suffix.
	if (LangOpts.CPlusPlus)
		CurPtr = LexUDSuffix(Result, CurPtr, false);

	// If a nul character existed in the character, warn about it.
	if (NulCharacter && !isLexingRawMode())
		Diag(NulCharacter, diag::null_in_char_or_string) << 0;

	// Update the location of token as well as BufferPtr.
	const char *TokStart = BufferPtr;
	FormTokenWithChars(Result, CurPtr, Kind);
	Result.setLiteralData(TokStart);
	return true;
}

static bool LexDot() {
	// Consume the initial period.
	scanner_advance();

	// Parse floating constants of the form `.25`.
	if (Character >= '0' && Character <= '9')
		return LexNumericConstant();

	// If we're not parsing an ellipsis then its a period.
	if (Character != '.')
		return ProduceToken(T_PERIOD);

	// Consume the second period.
	scanner_advance();

	// If we have a third period its an elipsis.
	if (Character == '.')
		return ConsumeToken(T_ELLIPSIS);

	// It wasn't an ellipsis.
	Token.kind = T_PERIOD;
	scanner_putback();
}

static bool LexLineComment() {
	// Line comment.
	// Even if Line comments are disabled (e.g. in C89 mode), we generally
	// want to lex this as a comment.  There is one problem with this though,
	// that in one particular corner case, this can change the behavior of the
	// resultant program.  For example, In  "foo //**/ bar", C89 would lex
	// this as "foo / bar" and languages with Line comments would lex it as
	// "foo".  Check to see if the character after the second slash is a '*'.
	// If so, we will lex that as a "/" instead of the start of a comment.
	// However, we never do this if we are just preprocessing.
	bool TreatAsComment =
	LineComment && (LangOpts.CPlusPlus || !LangOpts.TraditionalCPP);
	if (!TreatAsComment)
		if (!(PP && PP->isPreprocessedOutput()))
			TreatAsComment = getCharAndSize(CurPtr+SizeTmp, SizeTmp2) != '*';

	if (TreatAsComment) {
		if (SkipLineComment(Result, ConsumeChar(CurPtr, SizeTmp, Result),
							TokAtPhysicalStartOfLine))
			return true; // There is a token to return.

		// It is common for the tokens immediately after a // comment to be
		// whitespace (indentation for the next line).  Instead of going through
		// the big switch, handle it efficiently now.
		goto SkipIgnoredUnits;
	}
}

static bool LexBlockComment() {
	if (SkipBlockComment(Result, ConsumeChar(CurPtr, SizeTmp, Result),
						 TokAtPhysicalStartOfLine))
		return true; // There is a token to return.

	// We only saw whitespace, so just try again with this lexer.
	// (We manually eliminate the tail call to avoid recursion.)
	goto lex_next;
}

static bool _lexer_advance_preprocessor() {
	lex_start:;

	// Cache the current buffer head
	CurPtr = BufferPtr;

	// Small amounts of horizontal whitespace is very common between tokens.
	if (IsHorizontalWhitespace(Character)) {
		do
			++CurPtr;
		while (IsHorizontalWhitespace(Character));

		// If we are keeping whitespace and other tokens, just return what we just
		// skipped.  The next lexer invocation will return the token after the
		// whitespace.
		if (IsWhitespacePreserved) {
			scanner_anchor();
			Token.kind = T_WHITESPACE;
			return true;
		}

		BufferPtr = CurPtr;
		Token.flags.leading_space = true;
	}

	unsigned SizeTmp, SizeTmp2;   // Temporaries for use in cases below.

	scanner_advance();

	if (!IsVerticalWhitespace(Character))
		NewLinePtr = nullptr;

	switch (Character) {
	case 0:
		// Found end of file?
		if (CurPtr-1 == BufferEnd)
			return LexEndOfFile();

		if (!isLexingRawMode())
			Diag(CurPtr-1, diag::null_in_file);
		Result.setFlag(Token::LeadingSpace);
		if (SkipWhitespace(Result, CurPtr, TokAtPhysicalStartOfLine))
			return true; // KeepWhitespaceMode

		// We know the lexer hasn't changed, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;

	case '\r':
		if (Character == '\n')
			scanner_advance();
		[[fallthrough]];
	case '\n':
		// If we are inside a preprocessor directive and we see the end of line,
		// we know we are done with the directive, so return an EOD token.
		if (ParsingPreprocessorDirective) {
			// Done parsing the "line".
			ParsingPreprocessorDirective = false;

			// Restore comment saving mode, in case it was disabled for directive.
			if (PP)
				resetExtendedTokenMode();

			// Since we consumed a newline, we are back at the start of a line.
			IsAtStartOfLine = true;
			IsAtPhysicalStartOfLine = true;
			NewLinePtr = CurPtr - 1;

			Kind = tok::eod;
			break;
		}

		// No leading whitespace seen so far.
		Result.clearFlag(Token::LeadingSpace);

		if (SkipWhitespace(Result, CurPtr, TokAtPhysicalStartOfLine))
			return true; // KeepWhitespaceMode

		// We only saw whitespace, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;
	case ' ':
	case '\t':
	case '\f':
	case '\v':
	SkipHorizontalWhitespace:
		Result.setFlag(Token::LeadingSpace);
		if (SkipWhitespace(Result, CurPtr, TokAtPhysicalStartOfLine))
			return true; // KeepWhitespaceMode

	SkipIgnoredUnits:
		CurPtr = BufferPtr;

		// If the next token is obviously a // or /* */ comment, skip it efficiently
		// too (without going through the big switch stmt).
		if (CurPtr[0] == '/' && CurPtr[1] == '/' && !inKeepCommentMode() &&
			LineComment && (LangOpts.CPlusPlus || !LangOpts.TraditionalCPP)) {
			if (SkipLineComment(Result, CurPtr+2, TokAtPhysicalStartOfLine))
				return true; // There is a token to return.
			goto SkipIgnoredUnits;
		} else if (CurPtr[0] == '/' && CurPtr[1] == '*' && !inKeepCommentMode()) {
			if (SkipBlockComment(Result, CurPtr+2, TokAtPhysicalStartOfLine))
				return true; // There is a token to return.
			goto SkipIgnoredUnits;
		} else if (isHorizontalWhitespace(*CurPtr)) {
			goto SkipHorizontalWhitespace;
		}
		// We only saw whitespace, so just try again with this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;

		// C99 6.4.4.1: Integer Constants.
		// C99 6.4.4.2: Floating Constants.
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return LexNumericConstant();

		// Identifier
		// C23/C++17: UTF-8 literal
		// C11/C++11: UTF-16 literal
	case 'u':
		scanner_advance();
		Token.modifiers.encoding = scanner_consume('8') ? ENCODING_UTF8 : ENCODING_UTF16;

		switch (Character) {
		case '"': return LexStringLiteral();
		case '\'': return LexCharConstant();
		default: return LexIdentifierContinue();
		}

		// Identifier
		// C11/C++11: UTF-32 literal
	case 'U':
		scanner_advance();
		Token.modifiers.encoding = ENCODING_UTF32;

		switch (Character) {
		case '"': return LexStringLiteral();
		case '\'': return LexCharConstant();
		default: return LexIdentifierContinue();
		}

		// Identifier
		// Wide literal
	case 'L':
		scanner_advance();
		Token.modifiers.encoding = ENCODING_WIDE;

		switch (Character) {
		case '"': return LexStringLiteral();
		case '\'': return LexCharConstant();
		default: return LexIdentifierContinue();
		}

		// C99 6.4.2: Identifiers.
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': /* 'L' */ case 'M': case 'N':
	case 'O': case 'P': case 'Q': /* 'R' */ case 'S': case 'T': /* 'U' */
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': /* 'u' */
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		return LexIdentifierContinue();

		// C99 6.4.4: Character Constants.
	case '\'':
		Token.modifiers.encoding = ENCODING_NONE;
		return LexCharConstant();

		// C99 6.4.5: String Literals.
	case '"':
		Token.modifiers.encoding = ENCODING_NONE;
		return LexStringLiteral();

		// C99 6.4.6: Punctuators.
	case '?':
		return ConsumeToken(T_QUESTION);
	case '[':
		return ConsumeToken(T_LBRACKET);
	case ']':
		return ConsumeToken(T_RBRACKET);
	case '(':
		return ConsumeToken(T_LPAREN);
	case ')':
		return ConsumeToken(T_RPAREN);
	case '{':
		return ConsumeToken(T_LCURLY);
	case '}':
		return ConsumeToken(T_RCURLY);

		// Floating-point constant
		// Operators . ...
	case '.':
		if (Character >= 0 && Character <= '9')
			return LexNumericConstant();

		scanner_advance();

		//TODO: Ellipsis operator
		switch (Character) {
		default: return ProduceToken(T_PERIOD);
		}

		// Operators & && &=
	case '&':
		scanner_advance();

		switch (Character) {
		case '&': return ConsumeToken(T_LOGICAL_AND);
		case '=': return ConsumeToken(T_ASSIGN_BITWISE_AND);
		default: return ProduceToken(T_AMPERSAND);
		}

		// Operators * *=
	case '*':
		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_ASSIGN_MULTIPLY);
		default: return ProduceToken(T_ASTERISK);
		}

		// Operators + ++ +=
	case '+':
		scanner_advance();

		switch (Character) {
		case '+': return ConsumeToken(T_INCREMENT);
		case '=': return ConsumeToken(T_ASSIGN_ADD);
		default: return ProduceToken(T_PLUS);
		}

		// Operators - -- -> -=
	case '-':
		scanner_advance();

		//TODO: C++ arrow star operators
		switch (Character) {
		case '-': return ConsumeToken(T_DECREMENT);
		case '>': return ConsumeToken(T_ARROW);
		case '=': return ConsumeToken(T_ASSIGN_SUBTRACT);
		default: return ProduceToken(T_SUBTRACT);
		}

		// Operators ~
	case '~':
		return ConsumeToken(T_TILDE);

		// Operators ! !=
	case '!':
		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_NEQ);
		default: return ProduceToken(T_EXCLAIM);
		}

		// 6.4.9: Comments
		// Operators / /=
	case '/':
		scanner_advance();

		switch (Character) {
		case '/': return LexLineComment();
		case '*': return LexBlockComment();
		case '=': return ConsumeToken(T_ASSIGN_DIVIDE);
		default: return ProduceToken(T_DIVIDE);
		}

		// Operators % %=
		// Digraphs %> %: %:%:
	case '%':
		scanner_advance();

		if (IsDigraphsEnabled) {
			//TODO: Parse digraphs
		}

		switch (Character) {
		case '=': return ConsumeToken(T_ASSIGN_MODULO);
		default: return ProduceToken(T_PERCENT);
		}

		// Angled string literal
		// Operators < <= << <<=
		// Digraphs <: <%
	case '<':
		if (IsDigraphsEnabled) {
			//TODO: Parse digraphs
		}
		if (IsAngledStringLiteralEnabled)
			return LexAngledStringLiteral();

		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_LTE);
		case '<':
			scanner_advance();
			switch (Character) {
				//TODO: Detect C++ spaceship operator
			case '=': return ConsumeToken(T_ASSIGN_LSHIFT);
			default: return ProduceToken(T_LSHIFT);
			}
		default: return ProduceToken(T_LT);
		}

		// Operators > >= >> >>=
	case '>':
		scanner_advance();

		switch (Character) {
		case '>':
			scanner_advance();
			switch (Character) {
			case '=': return ConsumeToken(T_ASSIGN_RSHIFT);
			default: return ProduceToken(T_RSHIFT);
			}
		case '=': return ConsumeToken(T_GTE);
		default: return ProduceToken(T_GT);
		}

		// Operators ^ ^=
	case '^':
		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_ASSIGN_BITWISE_XOR);
		default: return ProduceToken(T_CARET);
		}

		// Operators | || |=
	case '|':
		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_ASSIGN_BITWISE_OR);
		case '|': return ConsumeToken(T_LOGICAL_OR);
		default: return ProduceToken(T_PIPE);
		}

		// Operators : ::
		// Digraphs :>
	case ':':
		scanner_advance();

		//TODO: Handle digraphs

		switch (Character) {
		case ':': return ConsumeToken(T_MODULE);
		case '|': return ConsumeToken(T_LOGICAL_OR);
		default: return ProduceToken(T_PIPE);
		}

	case ';':
		return ConsumeToken(T_SEMICOLON);

		// Operators = ==
	case '=':
		scanner_advance();

		switch (Character) {
		case '=': return ConsumeToken(T_EQ);
		default: return ProduceToken(T_ASSIGN);
		}

		// Operators ,
	case ',':
		return ConsumeToken(T_COMMA);

	case '#':
		scanner_advance();

		switch (Character) {
		case '#': return ConsumeToken(T_STRINGIFY);
		default: return ProduceToken(T_POUND);
		}

		//TODO: Detect preprocessor directives
		// We parsed a # character.  If this occurs at the start of the line,
		// it's actually the start of a preprocessing directive.  Callback to
		// the preprocessor to handle it.
		// TODO: -fpreprocessed mode??
		if (TokAtPhysicalStartOfLine && !LexingRawMode && !Is_PragmaLexer)
			goto lex_directive;

		// C99 6.4.3:
		// C++11 [lex.charset]p2:
		//	UCNs
	case '\\':
		if (uint32_t CodePoint = tryReadUCN(CurPtr, BufferPtr, &Result)) {
			if (CheckUnicodeWhitespace(Result, CodePoint, CurPtr)) {
				if (SkipWhitespace(Result, CurPtr, TokAtPhysicalStartOfLine))
					return true; // KeepWhitespaceMode

				// We only saw whitespace, so just try again with this lexer.
				// (We manually eliminate the tail call to avoid recursion.)
				goto lex_next;
			}

			return LexUnicodeIdentifierStart(Result, CodePoint, CurPtr);
		}

		Kind = tok::unknown;
		break;

	default:
		if (isxidstart(Character))
			return LexIdentifierContinue();

		if (isLexingRawMode() || ParsingPreprocessorDirective ||
			PP->isPreprocessedOutput()) {
			++CurPtr;
			Kind = tok::unknown;
			break;
		}

		// Non-ASCII characters tend to creep into source code unintentionally.
		// Instead of letting the parser complain about the unknown token,
		// just diagnose the invalid UTF-8, then drop the character.
		Diag(CurPtr, diag::err_invalid_utf8);

		BufferPtr = CurPtr+1;
		// We're pretending the character didn't exist, so just try again with
		// this lexer.
		// (We manually eliminate the tail call to avoid recursion.)
		goto lex_next;
	}

	// Update the location of token as well as BufferPtr.
	scanner_anchor();
	return true;

lex_directive:
	// We parsed a # character and it's the start of a preprocessing directive.

	FormTokenWithChars(Result, CurPtr, tok::hash);
	PP->HandleDirective(Result);

	if (PP->hadModuleLoaderFatalFailure())
		// With a fatal failure in the module loader, we abort parsing.
		return true;

	// We parsed the directive; lex a token with the new state.
	return false;

lex_next:
	Result.clearFlag(Token::NeedsCleaning);
	goto lex_start;
}

bool lexer_advance() {
	//TODO: Flag first token of line?
	//TODO: Flag whether token has leading whitespace?
	//TODO: Flag whether token is after empty macro?


	//TODO: Detect physical start of line?

	return _lexer_advance_preprocessor();
}
