#include "parser.h"

#include <functional>

const char CHAR_ESC   = '\\';
const char CHAR_SPACE = ' ';
const char CHAR_RET   = '\r';
const char CHAR_ZERO  = 0;

struct Command
{
	CommandType type;
	int parameterCount;
	const char* text;
	CommandHandler handler;
};

// если в команде есть пробел, перед ним надо поставить два слеша: "\\"

static const char* TEXT__GET      = "GET";
static const char* TEXT__LOG_DIR  = "LOG\\ DIR";
static const char* TEXT__LOG_DUMP = "LOG\\ DUMP";
static const char* TEXT__LOG_PAGE = "LOG\\ PAGE";
static const char* TEXT__SET      = "SET";

Command commands[] =
{
// -- команды тут должны следовать в алфавитном порядке!!!
// -- должно быть строгое соответствие между CommandType и описанных тут команд!!!

//    CommandType    | ParamCount | CommandText    | CommandHandler
	{ GetCommand,      1,           TEXT__GET,       nullptr },
	{ LogDirCommand,   0,           TEXT__LOG_DIR,   nullptr },
	{ LogDumpCommand,  0,           TEXT__LOG_DUMP,  nullptr },
	{ LogPageCommand,  0,           TEXT__LOG_PAGE,  nullptr },
	{ SetCommand,      2,           TEXT__SET,       nullptr },
};

// ------------------------------------------------------------------------

void Parser::registerHandler(CommandType commandType, CommandHandler&& handler)
{
	int index = static_cast<int>(commandType);
	if (index >= CommandCount) { return; }

	commands[index].handler = handler;
}

Parser::Result Parser::parse(char data, ParserState& state)
{
	int command = state.commandIndex();
	if (command >= CommandCount) { return FailResult; }

	Result result = IntermediateResult;

	switch (state.expectedToken()) {
	case CommandToken: {
		int commandIndex = command;
		for (; commandIndex < CommandCount; ++commandIndex) {
			Command& expectedCommand = commands[commandIndex];
			int index = state.index();
			char expectedChar = expectedCommand.text[index];
			if ((expectedChar == CHAR_ZERO) && (data == CHAR_RET)) {
				result = SuccessResult;
				break;
			} else
			if ((expectedChar == CHAR_ESC) && (data == expectedCommand.text[index + 1])) {
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
		if ((data == CHAR_ZERO) || (data == CHAR_RET)) { // ждали начала параметра, получили конец - ошибка
			result = FailResult;
			// проверить количество параметров
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
		if ((data == CHAR_ZERO) || data == (CHAR_RET)) { // закончили
			// TODO: проверить количество параметров

		} else {
			state.appendParameterChar(data);
		}
		break;
	}
	default:
		break;
	}

	if (result == SuccessResult) {
		Command& cmd = commands[command];
		if (cmd.handler) {
			cmd.handler(cmd.type, cmd.parameterCount, nullptr);
		}
	}

	return result;
}
