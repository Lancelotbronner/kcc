//
//  LexerTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-29.
//

import Testing
@preconcurrency import libkcc
import Darwin

@Suite(.serialized, .tags(.lexer))
struct LexerTests {

	@Test("+\\\\n=")
	func testEscapedNewline() async throws {
		//TODO: Migrate to scanner+lexer init (drop global Scanner)
		let code = "+\\\n="
		kcc_inmem(strdup(code), code.count + 1)
		lexer_advance(Lexer)
		#expect(Token.kind == T_ASSIGN_ADD)
		#expect(lexer_eof(Lexer))
	}

	@Test("int x = 25;")
	func testTokens() async throws {
		//TODO: Migrate to scanner+lexer init (drop global Scanner)
		let code = "int x = 25;"
		kcc_inmem(strdup(code), code.count + 1)
		lexer_advance(Lexer)
		#expect(Token.kind == T_INT)
		lexer_advance(Lexer)
		#expect(Token.kind == T_IDENTIFIER)
		lexer_advance(Lexer)
		#expect(Token.kind == T_ASSIGN)
		lexer_advance(Lexer)
		#expect(Token.kind == T_INTEGER_LITERAL)
		lexer_advance(Lexer)
		#expect(Token.kind == T_SEMICOLON)
		#expect(lexer_eof(Lexer))
	}

}
