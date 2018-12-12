// ManualParserSolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/optional/optional_io.hpp>

#include "../ManualLexer/CalcLexer.h"
#include "../ManualParser/CalcParser.h"

using namespace std;
using namespace calc;

int main()
{
	string inputString;
	getline(cin, inputString);

	CalcLexer lexer = CalcLexer(inputString);
	CCalcParser parser = CCalcParser(lexer);

	double parsedNumber = parser.Calculate();
	cout << "Parsed: " << parsedNumber << endl;

    return 0;
}

