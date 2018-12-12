#pragma once

#include <memory>
#include "../ManualLexer/Token.h"

namespace calc
{

struct Expression
{
	double value = 0;
	TokenType operation = TT_END;
	std::shared_ptr<Expression> left = nullptr;
	std::shared_ptr<Expression> right = nullptr;
};

}