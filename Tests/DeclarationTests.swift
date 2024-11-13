//
//  DeclarationTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-07-27.
//

import Testing
import libkcc

@Suite(.serialized, .tags(.parser, .declaration))
struct DeclarationTests {

	@Test("int x;")
	func testSingleDeclaration() async throws {
		let expression = "int x;"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

	@Test("int x = 5;")
	func testSingleDeclarationWithInitializer() async throws {
		let expression = "int x = 5;"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

	@Test("int x, y, z;")
	func testMultipleDeclaration() async throws {
		let expression = "int x, y, z;"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

	@Test("int *x, **y, ***z;")
	func testMultiplePointerDeclaration() async throws {
		let expression = "int *x, **y, ***z;"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

	@Test("int x[2], *y[SIZE], (*z)[SIZE+5];")
	func testMultipleArrayDeclaration() async throws {
		let expression = "int x[2], *y[SIZE], (*z)[SIZE+5];"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

	@Test("int (*x[4])()[3], **y(), ***z[2];")
	func testComplexDeclaration() async throws {
		let expression = "int (*x[4])()[3], **y(), ***z[2];"
		try expression.withCString {
			kcc_inmem($0, expression.count)
			let node = try #require(parser_parse())
			print_ast(node)
		}
	}

}
