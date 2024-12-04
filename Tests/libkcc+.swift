//
//  libkcc+.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

import Darwin
@preconcurrency import libkcc

extension token: @retroactive CustomStringConvertible {
	public var description: String {
		String(cString: token_nameof(kind))
	}
}

func scanner_init(_ scanner: scanner_t!, _ code: String) {
	scanner_init(scanner, strdup(code), code.count + 1)
}
