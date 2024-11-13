//
//  ScannerTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

import Testing
@preconcurrency import libkcc
import Darwin

@Suite(.tags(.scanner))
struct ScannerTests {

	@Test("+\\⏎=")
	func testEscapedNewline() async throws {
		let code = "+\\\n="
		let scanner = scanner_alloc()
		scanner_init(scanner, strdup(code), code.count + 1)
		#expect(scanner_peek(scanner) == Int8(bitPattern: UInt8(ascii: "+")))
		scanner_advance(scanner)
		#expect(scanner_peek(scanner) == Int8(bitPattern: UInt8(ascii: "=")))
		scanner_advance(scanner)
		#expect(scanner_eof(scanner))
	}

	@Test("+\\ ⏎=")
	func testEscapedNewlineWithSeparatingWhitespace() async throws {
		let code = "+\\ \n="
		let scanner = scanner_alloc()
		scanner_init(scanner, strdup(code), code.count + 1)
		#expect(scanner_peek(scanner) == Int8(bitPattern: UInt8(ascii: "+")))
		scanner_advance(scanner)
		#expect(scanner_peek(scanner) == Int8(bitPattern: UInt8(ascii: "=")))
		scanner_advance(scanner)
		#expect(scanner_eof(scanner))
	}

}
