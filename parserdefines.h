#ifndef PARSERDEFINES_H
#define PARSERDEFINES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <functional>

enum CommandType : char
{
	UnknownCommand = -1,

	GetCommand,				// Команды должны быть описаны в алфавитном порядке
	LogDirCommand,
	LogDumpCommand,
	LogPageCommand,
	SetCommand,

	CommandCount,			// обязательный последний параметр
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

using CommandHandler = std::function<void(CommandType type, int, char* params)>;

#endif // PARSERDEFINES_H
