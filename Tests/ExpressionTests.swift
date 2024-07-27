//
//  ExpressionTests.swift
//  kcc
//
//  Created by Christophe Bronner on 2024-07-20.
//

import XCTest
import libkcc

class ExpressionTests: XCTestCase {
	
	func testExpression() {
		let expression = "x + (short)5 * 3 * 4"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

}
