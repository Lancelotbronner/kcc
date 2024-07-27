//
//  token.cpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

#include <token.hpp>

using namespace kcc;

token::token(tokens::token_value &&value) : _value(std::move(value)) {}
