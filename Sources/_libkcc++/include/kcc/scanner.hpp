//
//  scanner.h
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

#include <kcc/token.hpp>

#include <istream>

namespace kcc {
	class scanner {
		std::string &_source;
		size_t _offset;
		token_flags _flags;
		token_modifiers _modifiers;
		std::string _literal_string;
		long double _literal_float;
		long long _literal_integer;

		int _peek() const;
		void _consume();
		bool _advance();
		token _token(token_kind kind);

	public:
		scanner(scanner const &) = delete;
		scanner &operator =(scanner const &) = delete;

		scanner(scanner &&other) noexcept = delete;
		scanner &operator =(scanner &&other) noexcept = delete;

		scanner(std::string &source) noexcept;

		std::string source() const;

		std::string literal_string() const;
		long double literal_float() const;
		long long literal_integer() const;

		bool is_whitespace();
		token scan_whitespace();

		bool is_eof();
		token scan_eof();
	};
}
