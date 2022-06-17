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

	static Result parse(char data, ParserState& state);

	static const char* parameter(size_t count, const char* parameters);
};

#endif // PARSER_H
