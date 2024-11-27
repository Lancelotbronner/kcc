//
//  ExpressionTests.swift
//  kcc
//
//  Created by Christophe Bronner on 2024-07-20.
//

import Testing
import libkcc
import Darwin

@Suite(.serialized, .tags(.parser, .expression))
struct ExpressionTests {

	@Test("x + (short)5 * 3 * 4")
	func testExpression() async throws {
		let expression = "x + (short)5 * 3 * 4"
		kcc_inmem(strdup(expression), expression.count)
		let node = try #require(parser_parse())
		print_ast(node)
	}

}
