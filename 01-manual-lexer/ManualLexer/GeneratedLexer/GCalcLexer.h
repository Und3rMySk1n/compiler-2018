#pragma once
#include <memory>
#include <iostream>
#include "lexertl/generator.hpp"
#include "lexertl/iterator.hpp"
#include "GToken.h"


namespace gcalc
{

	class GCalcLexer
	{
	public:
		GCalcLexer(std::string &sources);

		GToken Read();

	private:
		lexertl::rules GetRules();

		lexertl::state_machine m_lexer;
		std::unique_ptr<lexertl::smatch> m_results;
	};

}
