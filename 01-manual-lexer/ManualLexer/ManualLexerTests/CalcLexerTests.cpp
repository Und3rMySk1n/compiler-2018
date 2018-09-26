#include "catch2/catch.hpp"
#include "../ManualLexer/CalcLexer.h"
#include <vector>

using namespace std;
using namespace calc;

namespace calc
{
bool operator ==(const Token& a, const Token& b)
{
	return a.type == b.type && a.value == b.value;
}

string GetTokenName(TokenType type)
{
	switch (type)
	{
	case calc::TT_END:
		return "end";
	case calc::TT_ERROR:
		return "error";
	case calc::TT_NUMBER:
		return "number";
	case calc::TT_PLUS:
		return "+";
	}
	return "<UNEXPECTED!!!>";
}

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	stream << "Token(" << GetTokenName(token.type);
	if (token.value)
	{
		stream << ", " << *token.value;
	}
	stream << ")";
	return stream;
}
}

namespace
{

using TokenList = vector<Token>;

TokenList Tokenize(string text)
{
	TokenList results;
	CalcLexer lexer{ text };
	for (Token token = lexer.Read(); token.type != TT_END; token = lexer.Read())
	{
		results.emplace_back(move(token));
	}
	return results;
}

}

TEST_CASE("Can read one number", "[CalcLexer]") {
	REQUIRE(Tokenize("0") == TokenList{
		Token{ TT_NUMBER, "0" },
		});
	REQUIRE(Tokenize("1") == TokenList{
		Token{ TT_NUMBER, "1" },
		});
	REQUIRE(Tokenize("9876543210") == TokenList{
		Token{ TT_NUMBER, "9876543210" },
		});
}

TEST_CASE("Can read one operator", "[CalcLexer]") {
	REQUIRE(Tokenize("+") == TokenList{
		Token{ TT_PLUS },
		});
}

TEST_CASE("Can read expression tokens", "[CalcLexer]") {
	REQUIRE(Tokenize("45+9+28") == TokenList{
		Token{ TT_NUMBER, "45" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "9" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "28" },
		});

	REQUIRE(Tokenize("13+1-28*27/10") == TokenList{
		Token{ TT_NUMBER, "13" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "1" },
		Token{ TT_MINUS },
		Token{ TT_NUMBER, "28" },
		Token{ TT_MULTIPLICATION },
		Token{ TT_NUMBER, "27" },
		Token{ TT_DIVISION },
		Token{ TT_NUMBER, "10" },
		});
	

#if 1 // fractional number support
	REQUIRE(Tokenize("5+7.005") == TokenList{
		Token{ TT_NUMBER, "5" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "7.005" },
		});
	REQUIRE(Tokenize("1.005+43.54+1") == TokenList{
		Token{ TT_NUMBER, "1.005" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "43.54" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "1" },
		});
#endif
}

#if 1 // whitespace support
TEST_CASE("Can read one operator with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\t+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   \t\t+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\n+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   \n  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\t   \n  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+    ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+  \t\t   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+  \n\t   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   +   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t +  \t ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t\t +  \t ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t \t +  \n\t ") == TokenList{
		Token{ TT_PLUS },
		});
}

TEST_CASE("Can read one number with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  1") == TokenList{
		Token{ TT_NUMBER, "1" },
		});
	REQUIRE(Tokenize("\t4") == TokenList{
		Token{ TT_NUMBER, "4" },
		});
	REQUIRE(Tokenize("   \t\t3.2") == TokenList{
		Token{ TT_NUMBER, "3.2" },
		});
	REQUIRE(Tokenize("\n9") == TokenList{
		Token{ TT_NUMBER, "9" },
		});
	REQUIRE(Tokenize("   \n  15") == TokenList{
		Token{ TT_NUMBER, "15" },
		});
	REQUIRE(Tokenize("\t   \n  21.03") == TokenList{
		Token{ TT_NUMBER, "21.03" },
		});
	REQUIRE(Tokenize("0    ") == TokenList{
		Token{ TT_NUMBER, "0" },
		});
	REQUIRE(Tokenize("81  \t\t   ") == TokenList{
		Token{ TT_NUMBER, "81" },
		});
	REQUIRE(Tokenize("4.2  \n\t   ") == TokenList{
		Token{ TT_NUMBER, "4.2" },
		});
	REQUIRE(Tokenize("   7.9   ") == TokenList{
		Token{ TT_NUMBER, "7.9" },
		});
	REQUIRE(Tokenize("  \t 3  \t ") == TokenList{
		Token{ TT_NUMBER, "3" },
		});
	REQUIRE(Tokenize("  \t\t 9.001  \t ") == TokenList{
		Token{ TT_NUMBER, "9.001" },
		});
	REQUIRE(Tokenize("  \t \t 7  \n\t ") == TokenList{
		Token{ TT_NUMBER, "7" },
		});
}

TEST_CASE("Can read expression tokens with whitespaces") {
	REQUIRE(Tokenize("2 + 3") == TokenList{
		Token{ TT_NUMBER, "2" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "3" },
		});
	REQUIRE(Tokenize("\t0.52 + \n4") == TokenList{
		Token{ TT_NUMBER, "0.52" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "4" },
		});
	REQUIRE(Tokenize("\n+ \t7.1") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "7.1" },
		});
}
#endif

#if 1 
TEST_CASE("Cannot read number which starts with zero") {
	REQUIRE(Tokenize("0123456789") == TokenList{
		Token{ TT_ERROR },
		});
	REQUIRE(Tokenize("01.25") == TokenList{
		Token{ TT_ERROR },
		});
	REQUIRE(Tokenize("+01") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_ERROR },
		});
	REQUIRE(Tokenize("+00.32") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_ERROR },
		});
	REQUIRE(Tokenize("4+0521") == TokenList{
		Token{ TT_NUMBER, "4" },
		Token{ TT_PLUS },
		Token{ TT_ERROR },
		});
	REQUIRE(Tokenize("02+21") == TokenList{
		Token{ TT_ERROR },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "21" },
		});
	REQUIRE(Tokenize("02.4+5.3") == TokenList{
		Token{ TT_ERROR },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "5.3" },
		});
}
#endif

#if 1 // IDs support
TEST_CASE("Can read ID which starts with character") {
	REQUIRE(Tokenize("thisIsSampleIdWithLettersOnly") == TokenList{
		Token{ TT_ID, "thisIsSampleIdWithLettersOnly" },
		});
	REQUIRE(Tokenize("Exampl3W1thD1g1t5") == TokenList{
		Token{ TT_ID, "Exampl3W1thD1g1t5" },
		});
	REQUIRE(Tokenize("_exampleStartsWithUnderscore123") == TokenList{
		Token{ TT_ID, "_exampleStartsWithUnderscore123" }
		});
	REQUIRE(Tokenize("_12___132__strange___ID__") == TokenList{
		Token{ TT_ID, "_12___132__strange___ID__" }
		});
}

TEST_CASE("Can read expressions with IDs") {
	REQUIRE(Tokenize("leftSide + rightSide") == TokenList{
		Token{ TT_ID, "leftSide" },
		Token{ TT_PLUS },
		Token{ TT_ID, "rightSide" },
		});
	REQUIRE(Tokenize("one - 2") == TokenList{
		Token{ TT_ID, "one" },
		Token{ TT_MINUS },
		Token{ TT_NUMBER, "2" },
		});
	REQUIRE(Tokenize("variableWithDigits27 / 10.05") == TokenList{
		Token{ TT_ID, "variableWithDigits27" },
		Token{ TT_DIVISION },
		Token{ TT_NUMBER, "10.05" },
		});
}
#endif


TEST_CASE("Can read expressions with brackets") {
	REQUIRE(Tokenize("(27 + 13) - 175.5") == TokenList{
		Token{ TT_LEFT_ROUND_BRACKET },
		Token{ TT_NUMBER, "27" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "13" },
		Token{ TT_RIGHT_ROUND_BRACKET },
		Token{ TT_MINUS },
		Token{ TT_NUMBER, "175.5" },
		});

	REQUIRE(Tokenize("[one, 2, three]") == TokenList{
		Token{ TT_LEFT_SQUARE_BRACKET },
		Token{ TT_ID, "one" },
		Token{ TT_COMMA },
		Token{ TT_NUMBER, "2" },
		Token{ TT_COMMA },
		Token{ TT_ID, "three" },
		Token{ TT_RIGHT_SQUARE_BRACKET },
		});
}
