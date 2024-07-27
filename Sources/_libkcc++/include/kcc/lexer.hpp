//
//  scanner.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

#include <kcc/token.hpp>

#include <istream>

namespace kcc {
	class lexer {
	protected:
		std::string &_source;
		token _token;

		token _consume() noexcept;

	public:
		lexer(lexer const &) = delete;
		lexer &operator =(lexer const &) = delete;

		lexer(lexer &&other) noexcept = delete;
		lexer &operator =(lexer &&other) noexcept = delete;

		lexer(std::string &source) noexcept;

		token peek() const noexcept;
		void advance() noexcept;
	};
}
