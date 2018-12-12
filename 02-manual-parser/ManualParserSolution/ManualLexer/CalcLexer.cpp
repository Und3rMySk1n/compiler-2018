#include "CalcLexer.h"
#include <cctype>

// 1. ����� �� ����� ���������� � ����� (�������� .5), ������� �������
// 2. �����, ������� ���������� � ����, � ������������ ����������� ���������, ������ ���������� ����� � �������
// 3. ������������� ������� IsTabulation

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

bool IsBlank(char ch)
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

bool IsBreakingSymbol(char ch)
{
	/*
	* Returns true if given character is tabulation.
	*/
	switch (ch)
	{
	case ' ':
	case '\t':
	case '\n':
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}':
	case ',':
		return true;
	default:
		return false;
	}
}
}

CalcLexer::CalcLexer(std::string &sources)
	: m_sources(sources)
{
}

Token CalcLexer::Read(bool movePosition)
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
	size_t position = m_position;

	if (movePosition)
	{
		++m_position;
	}
	else
	{
		++position;
	}

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
		return Token{ TT_RIGHT_SQUARE_BRACKET };
	case '{':
		return Token{ TT_LEFT_CURLY_BRACKET };
	case '}':
		return Token{ TT_RIGHT_CURLY_BRACKET };
	case ',':
		return Token{ TT_COMMA };
	case '.':
		return (movePosition) ? ProcessErrorSymbol(next, m_position) : ProcessErrorSymbol(next, position);
	default:
		break;
	}

	if (IsDigit(next))
	{
		return (movePosition) ? ReadNumber(next, m_position) : ReadNumber(next, position);
	}

	if (IsIdCharacter(next))
	{
		return (movePosition) ? ReadId(next, m_position) : ReadId(next, position);
	}

	return Token{ TT_ERROR };
}

void CalcLexer::SkipSpaces()
{
	while (m_position < m_sources.size() && IsBlank(m_sources[m_position]))
	{
		++m_position;
	}
}

Token CalcLexer::ReadNumber(char head, size_t &position)
{
	/*
	 * Reads the tail of number token and returns this token.
	 * PRECONDITION: first character already read.
	 * POSTCONDITION: all number characters have been read.
	 */

	std::string value;
	value += head;

	int dotsCount = 0;
	bool hasNotAllowedSymbols = false;

	while (position < m_sources.size() && !IsBreakingSymbol(m_sources[position]))
	{
		if (!IsDigit(m_sources[position]) && m_sources[position] != '.')
		{
			hasNotAllowedSymbols = true;
		}

		if (m_sources[position] == '.')
		{
			dotsCount++;
		}

		value += m_sources[position];
		++position;
	}

	if ((value.length() > 1 && value.at(0) == '0' && value.at(1) != '.') 
		|| (dotsCount > 1)
		|| hasNotAllowedSymbols)
	{
		return Token{ TT_ERROR };
	}

	return Token{ TT_NUMBER, value };
}

Token CalcLexer::ReadId(char head, size_t &position)
{
	/*
	 * Reads the tail of ID token and returns this token.
	 * PRECONDITION: first character already read.
	 * POSTCONDITION: all valid characters have been read.
	 */

	std::string value;
	value += head;

	while (position < m_sources.size() && (IsIdCharacter(m_sources[position]) || IsDigit(m_sources[position])))
	{
		value += m_sources[position];
		++position;
	}

	return Token{ TT_ID, value };
}

Token CalcLexer::ProcessErrorSymbol(char head, size_t &position)
{
    /*
	 * Reads all symbols after an error symbol and
	 * returns one error token for whole ID.
     */

	std::string value;
	value += head;

	while (position < m_sources.size() && (!IsBreakingSymbol(m_sources[position])))
	{
		++position;
	}

	return Token{ TT_ERROR };
}

}
