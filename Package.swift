// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
	name: "kcc2",
	products: [
		.library(name: "libkcc", targets: ["libkcc"]),
	],
	targets: [
		.target(name: "libkcc"),
		.testTarget(name: "KccTests", dependencies: ["libkcc"], path: "Tests"),
		.executableTarget(name: "kcc2", dependencies: ["libkcc"]),
	],
	cLanguageStandard: .c2x,
	cxxLanguageStandard: .cxx2b
)
