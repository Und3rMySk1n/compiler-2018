// ManualParserSolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ManualLexer/CalcLexer.h"
#include <boost/optional/optional_io.hpp>

using namespace std;
using namespace calc;

int main()
{
	string inputString = "+";
	CalcLexer lexer = CalcLexer(inputString);
	Token token = lexer.Read();

	cout << "Token type: " << token.type << " and value: " << token.value << endl;
    return 0;
}

