//
//  DeclarationTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

import XCTest
import libkcc

class DeclarationTests: XCCompilerTestCase {

	func testSingleDeclaration() {
		let expression = "int x;"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

	func testSingleDeclarationWithInitializer() {
		let expression = "int x = 5;"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

	func testMultipleDeclaration() {
		let expression = "int x, y, z;"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

	func testMultiplePointerDeclaration() {
		let expression = "int *x, **y, ***z;"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

	func testMultipleArrayDeclaration() {
		let expression = "int x[2], *y[SIZE], (*z)[SIZE+5];"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

	func testComplexDeclaration() {
		let expression = "int (*x[4])()[3], **y(), ***z[2];"
		expression.withCString {
			kcc_inmem($0, expression.count)
			let node = parser_parse()
			XCTAssertNotNil(node)
			print_ast(node)
		}
	}

}
