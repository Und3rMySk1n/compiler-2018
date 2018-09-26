#pragma once

#include <string>
#include <boost/optional.hpp>

namespace gcalc
{

	enum {
		TK_NUMBER = 1,
		TK_ID,
		TK_PLUS,
		TK_MINUS,
		TK_DIVISION,
		TK_MULTIPLICATION,
		TK_LEFT_ROUND_BRACKET,
		TK_RIGHT_ROUND_BRACKET,
		TK_LEFT_SQUARE_BRACKET,
		TK_RIGHT_SQUARE_BRACKET,
		TK_LEFT_CURLY_BRACKET,
		TK_RIGHT_CURLY_BRACKET,
		TK_COMMA,
		TK_ERROR
	};

	struct GToken
	{
		size_t type;
		std::string value;
	};
}
