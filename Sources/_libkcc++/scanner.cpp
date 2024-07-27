//
//  scanner.cpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#include <kcc/scanner.hpp>

#include <cctype>

using namespace kcc;

scanner::scanner(std::string &source) noexcept : _source(source) {}

std::string scanner::source() const {
	return _source.substr(0, _offset);
}

std::string scanner::literal_string() const {
	return _literal_string;
};

long double scanner::literal_float() const {
	return _literal_float;
}

long long scanner::literal_integer() const {
	return _literal_integer;
}

bool scanner::is_whitespace() {
	return isspace(_peek());
}

token scanner::scan_whitespace() {
	_offset++;
	while (_advance() && isspace(_peek()));
	return _token(token_kind::whitespace);
}

bool scanner::is_eof() {
	return _source.empty();
}

token scanner::scan_eof() {
	return _token(token_kind::eof);
}

#pragma mark - private

int scanner::_peek() const {
	return _source[_offset];
}

void scanner::_consume() {
	_offset++;
}

bool scanner::_advance() {
	if (_offset >= _source.length()) return false;
	_consume();
	return true;
}

token scanner::_token(token_kind kind) {
	token tok(kind, _modifiers, _flags);
	_modifiers = {};
	_flags = {};
	_source.erase(0, _offset);
	_offset = 0;
	return tok;
}
