//
//  lexer.cpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#include <kcc/lexer.hpp>

#include <kcc/scanner.hpp>

using namespace kcc;

lexer::lexer(std::string &source) noexcept : _source(source) {}

token lexer::peek() const noexcept {
	return _token;
}

void lexer::advance() noexcept {
	_token = _consume();
}

#pragma mark - private

token lexer::_consume() noexcept {
	scanner scanner(_source);

	if (scanner.is_eof())
		return scanner.scan_eof();

	if (scanner.is_whitespace())
		return scanner.scan_whitespace();
}
