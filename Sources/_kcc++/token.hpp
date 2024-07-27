//
//  token.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

#include <token/tokens.hpp>

namespace kcc {
	class token {
		token_value _value;

	public:
		token() noexcept = default;
		token(token_value &&value);

		[[nodiscard]] token_value value() const;

		bool is_reserved() const;
		[[nodiscard]] token_reserved reserved() const;

		bool is_identifier() const;
		[[nodiscard]] token_identifier identifier() const;

		bool is_integer() const;
		[[nodiscard]] token_integer integer() const;

		bool is_string() const;
		[[nodiscard]] token_string string() const;

	};
}
