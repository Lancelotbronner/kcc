//
//  IntegerConstantTests.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-26.
//

import Testing
import libkcc

@Suite(.tags(.parser, .integerLiteral))
class IntegerConstantTests {

	func test_6_4_4_1_paragraph_3() async throws {
		/*
		0b11’10’11’01 /* 0b11101101 */
		’1’2 /* character constant ’1’ followed by integer constant 2,
		not the integer constant 12 */
		11’22 /* 1122 */
		0x’FFFF’FFFF /* invalid hexadecimal constant (’ cannot appear after 0x) */
		0x1’2’3’4AB’C’D /* 0x1234ABCD */
		*/
	}

}
