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
		let code = "+\\\n="
		kcc_inmem(strdup(code), code.count + 1)
		lexer_advance()
		#expect(Token.kind == T_ASSIGN_ADD)
		#expect(lexer_eof())
	}

	@Test("int x = 25;")
	func testTokens() async throws {
		let code = "int x = 25;"
		kcc_inmem(strdup(code), code.count + 1)
		lexer_advance()
		#expect(Token.kind == T_INT)
		lexer_advance()
		#expect(Token.kind == T_IDENTIFIER)
		lexer_advance()
		#expect(Token.kind == T_ASSIGN)
		lexer_advance()
		#expect(Token.kind == T_INTEGER_LITERAL)
		lexer_advance()
		#expect(Token.kind == T_SEMICOLON)
		#expect(lexer_eof())
	}

}
