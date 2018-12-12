#include "stdafx.h"
#include "CalcParser.h"

namespace calc
{
	using namespace std;


	double CCalcParser::Calculate()
	{
		shared_ptr<Expression> expression = this->ParseAddAndSubtract();
		return this->CalculateExpression(expression);
	}

	double CCalcParser::CalculateExpression(shared_ptr<Expression> expression)
	{
		if (expression->operation == TokenType::TT_NUMBER)
		{
			return expression->value;
		}

		this->CalculateExpression(expression->left);
		this->CalculateExpression(expression->right);

		switch (expression->operation)
		{
		case TokenType::TT_PLUS:
			expression->value = expression->left->value + expression->right->value;
			break;
		case TokenType::TT_MINUS:
			expression->value = expression->left->value - expression->right->value;
			break;
		case TokenType::TT_MULTIPLICATION:
			expression->value = expression->left->value * expression->right->value;
			break;
		case TokenType::TT_DIVISION:
			expression->value = expression->left->value / expression->right->value;
			break;
		case TokenType::TT_END:
			break;
		}

		return expression->value;
	}

	shared_ptr<Expression> CCalcParser::ParseAddAndSubtract()
	{
		shared_ptr<Expression> left = make_shared<Expression>();
		left = this->ParseMultiplyAndDivide();

		while (true)
		{
			Token token = m_lexer.Read();
			if (token.type == TokenType::TT_END)
			{
				return left;
			}

			switch (token.type)
			{
			case TokenType::TT_PLUS:
			case TokenType::TT_MINUS:
				break;
			default:
				return left;
			}

			shared_ptr<Expression> right = make_shared<Expression>();
			right = this->ParseMultiplyAndDivide();

			shared_ptr<Expression> expression = make_shared<Expression>();
			expression->left = left;
			expression->right = right;
			expression->operation = token.type;
			left = expression;
		}

		return left;
	}

	shared_ptr<Expression> CCalcParser::ParseMultiplyAndDivide()
	{
		shared_ptr<Expression> left = make_shared<Expression>();
		left = this->ParseAtom();

		while (true)
		{
			Token token = m_lexer.Read(false);
			if (token.type == TokenType::TT_END)
			{
				return left;
			}

			switch (token.type)
			{
			case TokenType::TT_MULTIPLICATION:
			case TokenType::TT_DIVISION:
				break;
			default:
				return left;
			}

			token = m_lexer.Read();
			shared_ptr<Expression> right = make_shared<Expression>();
			right = this->ParseAtom();

			shared_ptr<Expression> expression = make_shared<Expression>();
			expression->left = left;
			expression->right = right;
			expression->operation = token.type;
			left = expression;
		}

		return left;
	}

	shared_ptr<Expression> CCalcParser::ParseAtom()
	{
		shared_ptr<Expression> expression = make_shared<Expression>();
		Token token = m_lexer.Read();

		if (token.type != TokenType::TT_NUMBER)
		{
			throw std::invalid_argument("Expected number as atom.");
		}

		expression->operation = token.type;
		expression->value = stod(token.value);

		return expression;
	}
	
}
