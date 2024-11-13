//
//  Tags.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-10-24.
//

import Testing

extension Tag {
	@Tag static var declaration: Self
	@Tag static var expression: Self

	@Tag static var integerLiteral: Self

	@Tag static var scanner: Self
	@Tag static var lexer: Self
	@Tag static var parser: Self
}
