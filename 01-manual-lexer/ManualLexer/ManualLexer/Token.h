#pragma once

#include <string>
#include <optional>

namespace calc
{

enum TokenType
{
	TT_END = 0,
	TT_ERROR,
	TT_NUMBER,
	TT_ID,
	TT_PLUS,
	TT_MINUS,
	TT_DIVISION,
	TT_COMMA,
	TT_MULTIPLICATION,
	TT_LEFT_ROUND_BRACKET,
	TT_RIGHT_ROUND_BRACKET,
	TT_LEFT_SQUARE_BRACKET,
	TT_RIGHT_SQUARE_BRACKET,
	TT_LEFT_CURLY_BRACKET,
	TT_RIGHT_CURLY_BRACKET,

	// TODO: add other tokens here.
};

struct Token
{
	TokenType type = TT_END;
	std::optional<std::string> value;
};

}
