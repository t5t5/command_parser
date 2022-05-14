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
	CommandToken,
	SeparatorToken,
	ParameterToken,
};

static const int MaxCommandSize = 32;

using CommandHandler = std::function<void(CommandType type, size_t, const char* params)>;

#endif // PARSERDEFINES_H
