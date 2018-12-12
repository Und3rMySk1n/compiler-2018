#pragma once
#include "Token.h"

namespace calc
{

class CalcLexer
{
public:
	CalcLexer(std::string &sources);

	Token Read(bool movePosition = true);

private:
	void SkipSpaces();
	Token ReadNumber(char head, size_t &position);
	Token ReadId(char head, size_t &position);
	Token ProcessErrorSymbol(char head, size_t &position);

	std::string m_sources;
	size_t m_position = 0;
};

}
