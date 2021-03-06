#include "GCalcLexer.h"

using namespace gcalc;

// 1. Поддержка плавающих точек
// 2. Бросать ошибку на числах с буквами

GCalcLexer::GCalcLexer(std::string &sources)
{
	lexertl::rules rules = GetRules();
	lexertl::generator::build(rules, m_lexer);
	m_results = std::make_unique<lexertl::smatch>(sources.begin(), sources.end());
}

GToken GCalcLexer::Read()
{
	lexertl::lookup(m_lexer, *m_results);
	GToken token;

	token.type = (TokenType)m_results->id;
	if (token.type == TT_NUMBER || token.type == TT_ID)
	{
		token.value = m_results->str();
	}

	return token;
}

lexertl::rules GCalcLexer::GetRules()
{
	lexertl::rules rules;

	rules.push("[0][0-9]+", TT_ERROR);
	rules.push("\\.[0-9_a-zA-Z]+", TT_ERROR);
	rules.push("[0-9]+[_a-zA-Z]+([0-9]+)?", TT_ERROR);

	rules.push("[0-9]+(\\.[0-9]+)?", TT_NUMBER);
	rules.push("[_a-zA-Z][_a-zA-Z0-9]+", TT_ID);
	rules.push("\\+", TT_PLUS);
	rules.push("\\-", TT_MINUS);
	rules.push("\\*", TT_MULTIPLICATION);
	rules.push("\\/", TT_DIVISION);
	rules.push("\\,", TT_COMMA);

	rules.push("\\(", TT_LEFT_ROUND_BRACKET);
	rules.push("\\)", TT_RIGHT_ROUND_BRACKET);
	rules.push("\\[", TT_LEFT_SQUARE_BRACKET);
	rules.push("\\]", TT_RIGHT_SQUARE_BRACKET);
	rules.push("\\{", TT_LEFT_CURLY_BRACKET);
	rules.push("\\}", TT_RIGHT_CURLY_BRACKET);

	rules.push("[ \t\r\n]+", rules.skip());

	return rules;
}