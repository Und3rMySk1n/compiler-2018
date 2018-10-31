#include "GCalcLexer.h"

using namespace gcalc;

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

	token.type = m_results->id;
	if (token.type == TK_NUMBER || token.type == TK_ID)
	{
		token.value = m_results->str();
	}

	return token;
}

lexertl::rules GCalcLexer::GetRules()
{
	lexertl::rules rules;

	// поддержка плавающих запятых
	rules.push("[0][0-9]+", TK_ERROR);
	rules.push("[0-9]+", TK_NUMBER);
	rules.push("[_a-zA-Z][_a-zA-Z0-9]+", TK_ID);
	rules.push("\\+", TK_PLUS);
	rules.push("\\-", TK_MINUS);
	rules.push("\\*", TK_MULTIPLICATION);
	rules.push("\\/", TK_DIVISION);
	rules.push("\\,", TK_COMMA);

	rules.push("\\(", TK_LEFT_ROUND_BRACKET);
	rules.push("\\)", TK_RIGHT_ROUND_BRACKET);
	rules.push("\\[", TK_LEFT_SQUARE_BRACKET);
	rules.push("\\]", TK_RIGHT_SQUARE_BRACKET);
	rules.push("\\{", TK_LEFT_CURLY_BRACKET);
	rules.push("\\}", TK_RIGHT_CURLY_BRACKET);

	rules.push("[ \t\r\n]+", rules.skip());

	return rules;
}