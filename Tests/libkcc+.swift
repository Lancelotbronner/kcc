//
//  libkcc+.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-11-13.
//

@preconcurrency import libkcc

extension token: @retroactive CustomStringConvertible {
	public var description: String {
		String(cString: token_nameof(kind))
	}
}
