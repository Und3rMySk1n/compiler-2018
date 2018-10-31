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
		GToken{ TT_NUMBER, "0" },
	});
	REQUIRE(Tokenize("1") == TokenList{
		GToken{ TT_NUMBER, "1" },
	});
	REQUIRE(Tokenize("9876543210") == TokenList{
		GToken{ TT_NUMBER, "9876543210" },
	});
}

TEST_CASE("Generated lexer can read one operator", "[GCalcLexer]") {
	REQUIRE(Tokenize("+") == TokenList{
		GToken{ TT_PLUS },
	});

	REQUIRE(Tokenize("-") == TokenList{
		GToken{ TT_MINUS },
	});
}

TEST_CASE("Generated lexer can read expression tokens", "[GCalcLexer]") {
	REQUIRE(Tokenize("45+9+28") == TokenList{
		GToken{ TT_NUMBER, "45" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "9" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "28" },
	});

	REQUIRE(Tokenize("00") == TokenList{
		GToken{ TT_ERROR }
	});

	REQUIRE(Tokenize("13+1-28*27/10") == TokenList{
		GToken{ TT_NUMBER, "13" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "1" },
		GToken{ TT_MINUS },
		GToken{ TT_NUMBER, "28" },
		GToken{ TT_MULTIPLICATION },
		GToken{ TT_NUMBER, "27" },
		GToken{ TT_DIVISION },
		GToken{ TT_NUMBER, "10" },
	});
}

TEST_CASE("Generated lexer can read one operator with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  +") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("\t+") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("   \t\t+") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("\n+") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("   \n  +") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("\t   \n  +") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("+    ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("+  \t\t   ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("+  \n\t   ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("   +   ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("  \t +  \t ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("  \t\t +  \t ") == TokenList{
		GToken{ TT_PLUS },
	});
	REQUIRE(Tokenize("  \t \t +  \n\t ") == TokenList{
		GToken{ TT_PLUS },
	});
}

TEST_CASE("Generated lexer can read one number with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  1") == TokenList{
		GToken{ TT_NUMBER, "1" },
	});
	REQUIRE(Tokenize("\t4") == TokenList{
		GToken{ TT_NUMBER, "4" },
	});
	REQUIRE(Tokenize("\n9") == TokenList{
		GToken{ TT_NUMBER, "9" },
	});
	REQUIRE(Tokenize("   \n  15") == TokenList{
		GToken{ TT_NUMBER, "15" },
	});
	REQUIRE(Tokenize("0    ") == TokenList{
		GToken{ TT_NUMBER, "0" },
	});
	REQUIRE(Tokenize("81  \t\t   ") == TokenList{
		GToken{ TT_NUMBER, "81" },
	});
	REQUIRE(Tokenize("  \t 3  \t ") == TokenList{
		GToken{ TT_NUMBER, "3" },
	});
	REQUIRE(Tokenize("  \t \t 7  \n\t ") == TokenList{
		GToken{ TT_NUMBER, "7" },
	});
}

TEST_CASE("Generated lexer can read expression tokens with whitespaces") {
	REQUIRE(Tokenize("2 + 3") == TokenList{
		GToken{ TT_NUMBER, "2" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "3" },
	});
}

TEST_CASE("Generated lexer cannot read number which starts with zero") {
	REQUIRE(Tokenize("0123456789") == TokenList{
		GToken{ TT_ERROR },
	});
	REQUIRE(Tokenize("+01") == TokenList{
		GToken{ TT_PLUS },
		GToken{ TT_ERROR },
	});
	REQUIRE(Tokenize("4+0521") == TokenList{
		GToken{ TT_NUMBER, "4" },
		GToken{ TT_PLUS },
		GToken{ TT_ERROR },
	});
	REQUIRE(Tokenize("02+21") == TokenList{
		GToken{ TT_ERROR },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "21" },
	});
}

TEST_CASE("Generated lexer can read ID which starts with character") {
	REQUIRE(Tokenize("thisIsSampleIdWithLettersOnly") == TokenList{
		GToken{ TT_ID, "thisIsSampleIdWithLettersOnly" },
	});
	REQUIRE(Tokenize("Exampl3W1thD1g1t5") == TokenList{
		GToken{ TT_ID, "Exampl3W1thD1g1t5" },
	});
	REQUIRE(Tokenize("_exampleStartsWithUnderscore123") == TokenList{
		GToken{ TT_ID, "_exampleStartsWithUnderscore123" }
	});
	REQUIRE(Tokenize("_12___132__strange___ID__") == TokenList{
		GToken{ TT_ID, "_12___132__strange___ID__" }
	});
}

TEST_CASE("Generated lexer can read expressions with IDs") {
	REQUIRE(Tokenize("leftSide + rightSide") == TokenList{
		GToken{ TT_ID, "leftSide" },
		GToken{ TT_PLUS },
		GToken{ TT_ID, "rightSide" },
	});
	REQUIRE(Tokenize("one - 2") == TokenList{
		GToken{ TT_ID, "one" },
		GToken{ TT_MINUS },
		GToken{ TT_NUMBER, "2" },
	});
}

TEST_CASE("Generated lexer can read expressions with brackets") {
	REQUIRE(Tokenize("(27 + 13) - 175") == TokenList{
		GToken{ TT_LEFT_ROUND_BRACKET },
		GToken{ TT_NUMBER, "27" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "13" },
		GToken{ TT_RIGHT_ROUND_BRACKET },
		GToken{ TT_MINUS },
		GToken{ TT_NUMBER, "175" },
	});

	REQUIRE(Tokenize("[one, 2, three]") == TokenList{
		GToken{ TT_LEFT_SQUARE_BRACKET },
		GToken{ TT_ID, "one" },
		GToken{ TT_COMMA },
		GToken{ TT_NUMBER, "2" },
		GToken{ TT_COMMA },
		GToken{ TT_ID, "three" },
		GToken{ TT_RIGHT_SQUARE_BRACKET },
	});
}

TEST_CASE("Generated lexer can read fractional numbers") {
	REQUIRE(Tokenize("5+7.005") == TokenList{
		GToken{ TT_NUMBER, "5" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "7.005" },
	});
	REQUIRE(Tokenize("1.005+43.54+1") == TokenList{
		GToken{ TT_NUMBER, "1.005" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "43.54" },
		GToken{ TT_PLUS },
		GToken{ TT_NUMBER, "1" },
	});
	REQUIRE(Tokenize(".5778") == TokenList{
		GToken{ TT_ERROR }
	});
}

TEST_CASE("Generated lexer can not read numbers with ID characters") {
	REQUIRE(Tokenize("502someText") == TokenList{
		GToken{ TT_ERROR }
	});

	REQUIRE(Tokenize("502someText911") == TokenList{
		GToken{ TT_ERROR }
	});
}