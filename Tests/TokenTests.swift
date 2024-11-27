//
//  TokenTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-27.
//

import Testing
@preconcurrency import libkcc
import Darwin

@Suite(.serialized, .tags(.token))
struct TokenTests {

	@Test("keywords")
	func keywords() async throws {
		// ensure the multi-character horrors I've commited still works
		// the keywords themselves aren't really a problem,
		// mostly the lengths and keywords where a 4-char group starts with a number (like _DecimalX)
		#expect(token_keyword("do", 2) == kdo)
		#expect(token_keyword("for", 3) == T_FOR)
		#expect(token_keyword("bool", 4) == T_BOOL)
		#expect(token_keyword("const", 5) == T_CONST)
		#expect(token_keyword("double", 6) == T_DOUBLE)
		#expect(token_keyword("nullptr", 7) == T_NULLPTR)
		#expect(token_keyword("unsigned", 8) == T_UNSIGNED)
		#expect(token_keyword("_Noreturn", 9) == knoreturn)
		#expect(token_keyword("_Imaginary", 10) == kimaginary)
		#expect(token_keyword("_Decimal32", 10) == T_DECIMAL32)
		#expect(token_keyword("_Decimal64", 10) == T_DECIMAL64)
		#expect(token_keyword("_Decimal128", 11) == T_DECIMAL128)
		#expect(token_keyword("thread_local", 12) == kthread_local)
		#expect(token_keyword("typeof_unqual", 13) == ktypeof_unqual)
		#expect(token_keyword("_Static_assert", 14) == kstatic_assert)
	}

}
