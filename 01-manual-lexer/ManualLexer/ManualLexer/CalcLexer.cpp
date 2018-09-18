#include "CalcLexer.h"
#include <cctype>

namespace calc
{
namespace
{
bool IsDigit(char ch)
{
	/*
	 * Returns true if given character is digit.
	 */
	switch (ch)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;
	default:
		return false;
	}
}

bool IsIdCharacter(char ch)
{
	/*
	 * Returns true if given character is valid ID character.
	 */
	if (isalpha(ch) || ch == '_')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsTabulation(char ch)
{
	/*
	 * Returns true if given character is tabulation.
	 */
	switch (ch)
	{
	case ' ':
	case '\t':
	case '\n':
		return true;
	default:
		return false;
	}
}
}

CalcLexer::CalcLexer(std::string_view sources)
	: m_sources(sources)
{
}

Token CalcLexer::Read()
{
	/*
     * Reads next token from input string with following steps:
	 * 1) skips whitespace characters
	 * 2) checks for the end of input
	 * 3) checks first character to select token type
	 * 4) if token may have several characters, read them all
	 */

	SkipSpaces();

	if (m_position >= m_sources.size())
	{
		return Token{ TT_END };
	}

	char next = m_sources[m_position];
	++m_position;

	switch (next)
	{
	case '+':
		return Token{ TT_PLUS };
	case '-':
		return Token{ TT_MINUS };
	case '*':
		return Token{ TT_MULTIPLICATION };
	case '/':
		return Token{ TT_DIVISION };
	case '(':
		return Token{ TT_LEFT_ROUND_BRACKET };
	case ')':
		return Token{ TT_RIGHT_ROUND_BRACKET };
	case '[':
		return Token{ TT_LEFT_SQUARE_BRACKET };
	case ']':
		return Token{ TT_LEFT_SQUARE_BRACKET };
	case '{':
		return Token{ TT_LEFT_CURLY_BRACKET };
	case '}':
		return Token{ TT_LEFT_CURLY_BRACKET };
	case '.':
		return ReadNumber(next);
	default:
		break;
	}

	if (IsDigit(next))
	{
		return ReadNumber(next);
	}

	if (IsIdCharacter(next))
	{
		return ReadId(next);
	}

	return Token{ TT_ERROR };
}

void CalcLexer::SkipSpaces()
{
	while (m_position < m_sources.size() && IsTabulation(m_sources[m_position]))
	{
		++m_position;
	}
}

Token CalcLexer::ReadNumber(char head)
{
	/*
	 * Reads the tail of number token and returns this token.
	 * PRECONDITION: first character already read.
	 * POSTCONDITION: all number characters have been read.
	 */

	std::string value;
	value += head;

	while (m_position < m_sources.size() && (IsDigit(m_sources[m_position]) || m_sources[m_position] == '.'))
	{
		value += m_sources[m_position];
		++m_position;
	}

	if (value.length() > 1 && value.at(0) == '0' && value.at(1) != '.')
	{
		return Token{ TT_ERROR };
	}

	return Token{ TT_NUMBER, value };
}

Token CalcLexer::ReadId(char head)
{
	/*
	 * Reads the tail of ID token and returns this token.
	 * PRECONDITION: first character already read.
	 * POSTCONDITION: all valid characters have been read.
	 */

	std::string value;
	value += head;

	while (m_position < m_sources.size() && (IsIdCharacter(m_sources[m_position]) || IsDigit(m_sources[m_position])))
	{
		value += m_sources[m_position];
		++m_position;
	}

	return Token{ TT_ID, value };
}

}
