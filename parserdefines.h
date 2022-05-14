#ifndef PARSERDEFINES_H
#define PARSERDEFINES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

enum CommandType : char
{
	UnknownCommand = -1,
	GetCommand,
	LogDirCommand,
	LogDumpCommand,
	LogPageCommand,
	SetCommand,

	CommandCount,
};

enum TokenType : char
{
	UnknownToken,
	CommandToken,
	SeparatorToken,
	ParameterToken,
	EndToken,
};

static const int MaxCommandSize = 32;

#endif // PARSERDEFINES_H
