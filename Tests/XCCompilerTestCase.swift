//
//  XCCompilerTestCase.swift
//  kcc2
//
//  Created by Christophe Bronner on 2024-08-13.
//

import XCTest
import libkcc

class XCCompilerTestCase: XCTestCase {

	var diagnostics: [diagnostic] = []

	var lastDiagnostic: diagnostic? {
		_read {
			guard !diagnostics.isEmpty else { yield nil }
			yield diagnostics[diagnostics.endIndex - 1]
		}
	}

	override func setUp() {
		let data = Unmanaged.passRetained(self).toOpaque()
		diagnostics_register(data) { data, diagnostic_ptr in
			let test = Unmanaged<Self>.fromOpaque(data).takeUnretainedValue()
			test.diagnostics.append(diagnostic_ptr.pointee)
		}
	}

}
