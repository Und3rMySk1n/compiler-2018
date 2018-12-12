#pragma once

#include "../ManualLexer/CalcLexer.h"
#include "Expression.h"

namespace calc
{

class CCalcParser
{
public:
	CCalcParser(calc::CalcLexer lexer)
		: m_lexer(lexer) {}

	double Calculate();

	~CCalcParser() {}

private:
	double CalculateExpression(std::shared_ptr<Expression> expression);

	std::shared_ptr<Expression> ParseAddAndSubtract();
	std::shared_ptr<Expression> ParseMultiplyAndDivide();
	std::shared_ptr<Expression> ParseAtom();

	CalcLexer m_lexer;
};

}

