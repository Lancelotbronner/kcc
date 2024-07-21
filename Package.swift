// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
	name: "kcc",
	products: [
		.library(name: "libkcc", targets: ["libkcc"]),
	],
	targets: [
		.target(name: "libkcc"),
		.testTarget(name: "Tests", dependencies: ["libkcc"], path: "Tests"),
		.executableTarget(name: "kcc", dependencies: ["libkcc"]),
	],
	cLanguageStandard: .c2x,
	cxxLanguageStandard: .cxx2b
)
