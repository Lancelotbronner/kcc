//
//  token.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#pragma once

#include <kcc/token_kind.hpp>
#include <kcc/token_modifiers.hpp>

#include <string>
#include <variant>

namespace kcc {
	struct token_flags {

	};

	class token {
		token_kind _kind;
		token_modifiers _modifiers;
		token_flags _flags;

	public:
		token() noexcept = default;

		token(token_kind kind, token_modifiers modifiers, token_flags flags) noexcept;

		token_kind kind() const noexcept;
		token_modifiers modifiers() const noexcept;
		token_flags flags() const noexcept;
	};
}
