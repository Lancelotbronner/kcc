// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
	name: "kcc",
	targets: [
//		.systemLibrary(
//			name: "boost",
//			providers: [
//				.brew(["boost"]),
//			]),

		.executableTarget(
			name: "kcc",
//			dependencies: ["boost"],
			resources: [
				.process("lex.y"),
				.process("lex.l"),
			],
			cSettings: [
				.headerSearchPath("util"),
				.headerSearchPath("."),
				.headerSearchPath("DerivedSources"),
				//TODO: Turn into a system library when possible
				.unsafeFlags(["-I/opt/homebrew/include"]),
			]),
	],
//	cLanguageStandard: .c2x,
	cxxLanguageStandard: .cxx20
)
