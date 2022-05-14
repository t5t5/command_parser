#ifndef PARSER_H
#define PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "parserstate.h"

class Parser
{
public:

	enum Result
	{
		FailResult,
		SuccessResult,
		IntermediateResult,
	};

	static void registerHandler(CommandType commandType, CommandHandler&& handler);
	static Result parse(char data, ParserState& state);
};

#endif // PARSER_H
