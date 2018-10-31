#pragma once

#include <string>
#include <boost/optional.hpp>

namespace gcalc
{

	enum TokenType 
	{
		TT_NUMBER = 1,
		TT_ID,
		TT_PLUS,
		TT_MINUS,
		TT_DIVISION,
		TT_MULTIPLICATION,
		TT_LEFT_ROUND_BRACKET,
		TT_RIGHT_ROUND_BRACKET,
		TT_LEFT_SQUARE_BRACKET,
		TT_RIGHT_SQUARE_BRACKET,
		TT_LEFT_CURLY_BRACKET,
		TT_RIGHT_CURLY_BRACKET,
		TT_COMMA,
		TT_ERROR
	};

	struct GToken
	{
		TokenType type;
		std::string value;
	};
}
