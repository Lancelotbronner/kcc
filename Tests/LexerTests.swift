//
//  LexerTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-29.
//

import Testing
@preconcurrency import libkcc
import Darwin

@Suite(.tags(.lexer))
struct LexerTests {

	@Test("+\\\\n=")
	func testEscapedNewline() async throws {
		let code = "+\\\n="
		kcc_inmem(strdup(code), code.count + 1)
		scan()
		scan()
		#expect(Token.kind == T_ASSIGN_ADD)
		#expect(kcc_line() == 2)
		#expect(kcc_column() == 1)
	}

	@Test("int x = 25;")
	func testTokens() async throws {
		let code = "int x = 25;"
		kcc_inmem(strdup(code), code.count + 1)
		lexer_init()
		while lexer_advance() {
			print(Token)
		}
	}

}
