#include "parser.h"

#include <cstring>
#include <functional>

const char CHAR_ESC   = '\\';
const char CHAR_SPACE = ' ';
const char CHAR_RET   = '\r';
const char CHAR_LF    = '\n';
const char CHAR_ZERO  = 0;

struct Command
{
	CommandType type;
	size_t parameterCount;
	const char* text;
};

// если в команде есть пробел, перед ним надо поставить два слеша: "\\"

static const char* TEXT__GET      = "GET";
static const char* TEXT__GKHZ      = "GKHZ";
static const char* TEXT__LOG_CLEAN = "LOG\\ CLEAN";
static const char* TEXT__LOG_DIR  = "LOG\\ DIR";
static const char* TEXT__LOG_DUMP = "LOG\\ DUMP";
static const char* TEXT__LOG_PAGE = "LOG\\ PAGE";
static const char* TEXT__SET      = "SET";

Command commands[] =
{
// -- команды тут должны следовать в алфавитном порядке!!!
// -- должно быть строгое соответствие между CommandType и описанных тут команд!!!

//    CommandType    | ParamCount | CommandText
	{ GetCommand,      1,           TEXT__GET      },
	{ GkhzCommand,     0,           TEXT__GKHZ,    },
	{ LogCleanCommand, 0,           TEXT__LOG_CLEAN },
	{ LogDirCommand,   0,           TEXT__LOG_DIR  },
	{ LogDumpCommand,  0,           TEXT__LOG_DUMP },
	{ LogPageCommand,  0,           TEXT__LOG_PAGE },
	{ SetCommand,      2,           TEXT__SET      },
};

// ------------------------------------------------------------------------

static bool isCrLf(char c)
{
	return (c == CHAR_RET) || (c == CHAR_LF);
}

Parser::Result Parser::parse(char data, ParserState& state)
{
	int commandIndex = state.commandIndex();
	if (commandIndex >= CommandCount) { return FailResult; }

	Result result = IntermediateResult;

	const Command& command = commands[commandIndex];

	switch (state.expectedToken()) {
	case CommandToken: {
		for (; commandIndex < CommandCount; ++commandIndex) {
			const Command& command = commands[commandIndex];
			int index = state.index();
			char expectedChar = command.text[index];
			if ((expectedChar == CHAR_ZERO) && isCrLf(data)) {
				result = SuccessResult;
				break;
			} else
			if ((expectedChar == CHAR_ESC) && (data == command.text[index + 1])) {
				state.increaseIndex();
				state.increaseIndex();
				break;
			} else
			if ((data == CHAR_SPACE) && (expectedChar == CHAR_ZERO)) {
				state.setExpectedToken(SeparatorToken);
				break;
			} else
			if (data == expectedChar) {
				state.increaseIndex();
				break;
			} else
			if (data > expectedChar) {
				continue;
			} else
			if (data < expectedChar) {
				result = FailResult;		// уже точно нераспознанная команда
				break;
			}
		}
		if (result == FailResult) { return FailResult; }
		state.setCommandIndex(commandIndex);
		break;
	}
	case SeparatorToken: {
		if (data == CHAR_SPACE) { // параметр еще не начался
			// ничего не делаем, ждем дальше
		} else
		if ((data == CHAR_ZERO) || isCrLf(data)) { // ждали начала параметра, получили конец
//			// --- блок 1 - начало
//			// если после последнего параметра не должно быть пробелов,
//			// код в блоке 1 должен быть раскомменчен, а код в блоке 2 закомменчен
//			result = FailResult;
//			// --- блок 1 -  конец

			// --- блок 2 - начало
			result = (state.parameterCount() == command.parameterCount)
				? SuccessResult
				: FailResult;
			// --- блок 2 -  конец
		} else {
			// приняли символ
			state.nextParam();
			state.appendParameterChar(data);
			state.setExpectedToken(ParameterToken);
		}
		break;
	}
	case ParameterToken: {
		if (data == CHAR_SPACE) { // параметр закончился
			state.setExpectedToken(SeparatorToken);
		} else
		if ((data == CHAR_ZERO) || isCrLf(data)) { // закончили
			result = (state.parameterCount() == command.parameterCount)
				? SuccessResult
				: FailResult;
		} else {
			state.appendParameterChar(data);
		}
		break;
	}
	default:
		break;
	}

	if (result == SuccessResult) {
		bool ok = state.process();
		if (!ok) { result = FailResult; }
	}

	return result;
}

const char* Parser::parameter(size_t count, const char* parameters)
{
	if (count == 0) { return parameters; }
	size_t offset = 0;
	for (size_t i = 0; i < count; ++i) {
		offset += strlen(parameters + offset) + 1;
	}
	return &parameters[offset];
}
