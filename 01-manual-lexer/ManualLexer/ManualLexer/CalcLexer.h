#pragma once
#include "Token.h"

namespace calc
{

class CalcLexer
{
public:
	CalcLexer(std::string &sources);

	Token Read();

private:
	void SkipSpaces();
	Token ReadNumber(char head);
	Token ReadId(char head);
	Token ProcessErrorSymbol(char head);

	std::string m_sources;
	size_t m_position = 0;
};

}
