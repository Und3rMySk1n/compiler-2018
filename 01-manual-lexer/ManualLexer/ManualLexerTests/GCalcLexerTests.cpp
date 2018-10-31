#include "catch2/catch.hpp"
#include "../GeneratedLexer/GCalcLexer.h"
#include <vector>

using namespace std;
using namespace gcalc;

namespace gcalc
{
	bool operator ==(const GToken& a, const GToken& b)
	{
		return a.type == b.type && a.value == b.value;
	}
}

namespace
{
	using TokenList = vector<GToken>;

	TokenList Tokenize(string text)
	{
		TokenList results;
		GCalcLexer lexer(text);
		for (GToken token = lexer.Read(); token.type != 0; token = lexer.Read())
		{
			results.emplace_back(move(token));
		}
		return results;
	}

}

TEST_CASE("Generated lexer can read one number", "[GCalcLexer]") {
	REQUIRE(Tokenize("0") == TokenList{
		GToken{ TK_NUMBER, "0" },
	});
	REQUIRE(Tokenize("1") == TokenList{
		GToken{ TK_NUMBER, "1" },
	});
	REQUIRE(Tokenize("9876543210") == TokenList{
		GToken{ TK_NUMBER, "9876543210" },
	});
}

TEST_CASE("Generated lexer can read one operator", "[GCalcLexer]") {
	REQUIRE(Tokenize("+") == TokenList{
		GToken{ TK_PLUS },
	});

	REQUIRE(Tokenize("-") == TokenList{
		GToken{ TK_MINUS },
	});
}

TEST_CASE("Generated lexer can read expression tokens", "[GCalcLexer]") {
	REQUIRE(Tokenize("45+9+28") == TokenList{
		GToken{ TK_NUMBER, "45" },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "9" },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "28" },
	});

	REQUIRE(Tokenize("00") == TokenList{
		GToken{ TK_ERROR }
	});

	REQUIRE(Tokenize("13+1-28*27/10") == TokenList{
		GToken{ TK_NUMBER, "13" },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "1" },
		GToken{ TK_MINUS },
		GToken{ TK_NUMBER, "28" },
		GToken{ TK_MULTIPLICATION },
		GToken{ TK_NUMBER, "27" },
		GToken{ TK_DIVISION },
		GToken{ TK_NUMBER, "10" },
	});
}

TEST_CASE("Generated lexer can read one operator with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  +") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("\t+") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("   \t\t+") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("\n+") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("   \n  +") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("\t   \n  +") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("+    ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("+  \t\t   ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("+  \n\t   ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("   +   ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("  \t +  \t ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("  \t\t +  \t ") == TokenList{
		GToken{ TK_PLUS },
	});
	REQUIRE(Tokenize("  \t \t +  \n\t ") == TokenList{
		GToken{ TK_PLUS },
	});
}

TEST_CASE("Generated lexer can read one number with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  1") == TokenList{
		GToken{ TK_NUMBER, "1" },
	});
	REQUIRE(Tokenize("\t4") == TokenList{
		GToken{ TK_NUMBER, "4" },
	});
	REQUIRE(Tokenize("\n9") == TokenList{
		GToken{ TK_NUMBER, "9" },
	});
	REQUIRE(Tokenize("   \n  15") == TokenList{
		GToken{ TK_NUMBER, "15" },
	});
	REQUIRE(Tokenize("0    ") == TokenList{
		GToken{ TK_NUMBER, "0" },
	});
	REQUIRE(Tokenize("81  \t\t   ") == TokenList{
		GToken{ TK_NUMBER, "81" },
	});
	REQUIRE(Tokenize("  \t 3  \t ") == TokenList{
		GToken{ TK_NUMBER, "3" },
	});
	REQUIRE(Tokenize("  \t \t 7  \n\t ") == TokenList{
		GToken{ TK_NUMBER, "7" },
	});
}

TEST_CASE("Generated lexer can read expression tokens with whitespaces") {
	REQUIRE(Tokenize("2 + 3") == TokenList{
		GToken{ TK_NUMBER, "2" },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "3" },
	});
}

TEST_CASE("Generated lexer cannot read number which starts with zero") {
	REQUIRE(Tokenize("0123456789") == TokenList{
		GToken{ TK_ERROR },
	});
	REQUIRE(Tokenize("+01") == TokenList{
		GToken{ TK_PLUS },
		GToken{ TK_ERROR },
	});
	REQUIRE(Tokenize("4+0521") == TokenList{
		GToken{ TK_NUMBER, "4" },
		GToken{ TK_PLUS },
		GToken{ TK_ERROR },
	});
	REQUIRE(Tokenize("02+21") == TokenList{
		GToken{ TK_ERROR },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "21" },
	});
}

TEST_CASE("Generated lexer can read ID which starts with character") {
	REQUIRE(Tokenize("thisIsSampleIdWithLettersOnly") == TokenList{
		GToken{ TK_ID, "thisIsSampleIdWithLettersOnly" },
	});
	REQUIRE(Tokenize("Exampl3W1thD1g1t5") == TokenList{
		GToken{ TK_ID, "Exampl3W1thD1g1t5" },
	});
	REQUIRE(Tokenize("_exampleStartsWithUnderscore123") == TokenList{
		GToken{ TK_ID, "_exampleStartsWithUnderscore123" }
	});
	REQUIRE(Tokenize("_12___132__strange___ID__") == TokenList{
		GToken{ TK_ID, "_12___132__strange___ID__" }
	});
}

TEST_CASE("Generated lexer can read expressions with IDs") {
	REQUIRE(Tokenize("leftSide + rightSide") == TokenList{
		GToken{ TK_ID, "leftSide" },
		GToken{ TK_PLUS },
		GToken{ TK_ID, "rightSide" },
	});
	REQUIRE(Tokenize("one - 2") == TokenList{
		GToken{ TK_ID, "one" },
		GToken{ TK_MINUS },
		GToken{ TK_NUMBER, "2" },
	});
}

TEST_CASE("Generated lexer can read expressions with brackets") {
	REQUIRE(Tokenize("(27 + 13) - 175") == TokenList{
		GToken{ TK_LEFT_ROUND_BRACKET },
		GToken{ TK_NUMBER, "27" },
		GToken{ TK_PLUS },
		GToken{ TK_NUMBER, "13" },
		GToken{ TK_RIGHT_ROUND_BRACKET },
		GToken{ TK_MINUS },
		GToken{ TK_NUMBER, "175" },
	});

	REQUIRE(Tokenize("[one, 2, three]") == TokenList{
		GToken{ TK_LEFT_SQUARE_BRACKET },
		GToken{ TK_ID, "one" },
		GToken{ TK_COMMA },
		GToken{ TK_NUMBER, "2" },
		GToken{ TK_COMMA },
		GToken{ TK_ID, "three" },
		GToken{ TK_RIGHT_SQUARE_BRACKET },
	});
}