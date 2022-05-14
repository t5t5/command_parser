#include <QtTest/QTest>
#include <QDebug>

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

const char CHAR_ESC   = '\\';
const char CHAR_SPACE = ' ';
const char CHAR_RET   = '\r';
const char CHAR_ZERO  = 0;

struct Command
{
	CommandType type;
	int parameterCount;
	const char text[MaxCommandSize];
};

Command commands[] =
{
	{ GetCommand, 1, "GET" },
	{ LogDirCommand, 0, "LOG\\ DIR" },
	{ LogDumpCommand, 0, "LOG\\ DUMP" },
	{ LogPageCommand, 0, "LOG\\ PAGE" },
	{ SetCommand, 2, "SET" },
};

class ParserState
{
public:
	ParserState() = default;

	inline void clear()
	{
		m_index = 0;
		m_parameterIndex = 0;
		m_parameterCount = 0;
		m_commandType = GetCommand;
		m_expectedToken = CommandToken;
	}

	inline int index() const { return m_index; }
	inline void setIndex(int index) { m_index = index; }
	inline void increaseIndex() { ++m_index; }

	inline int parameterIndex() const { return m_parameterIndex; }
	inline void setParameterIndex(int index) { m_parameterIndex = index; }

	inline int parameterCount() const { return m_parameterCount; }
	inline void setParameterCount(int count) { m_parameterCount = count; }

	inline CommandType commandType() const { return m_commandType; }
	inline void setCommandType(CommandType type) { m_commandType = type; }

	inline int commandIndex() const { return static_cast<int>(m_commandType); }
	inline void setCommandIndex(int index) { m_commandType = static_cast<CommandType>(index); }

	inline TokenType expectedToken() const { return m_expectedToken; }
	inline void setExpectedToken(TokenType type) { m_expectedToken = type; }

	inline bool appendParameterChar(char data)
	{
		if ((m_parameterIndex + 1) == MaxCommandSize) { return false; }
		m_params[m_parameterIndex + 0] = data;
		m_params[m_parameterIndex + 1] = CHAR_ZERO;
		++m_parameterIndex;
		return true;
	}
	inline void nextParam() { ++m_parameterCount; ++m_parameterIndex; }

private:
	int m_index = 0;
	int m_parameterIndex = 0;
	int m_parameterCount = 0;
	CommandType m_commandType = GetCommand;
	TokenType m_expectedToken = CommandToken;
	char m_params[MaxCommandSize];
};

struct Parser
{
	enum Result
	{
		FailResult,
		SuccessResult,
		IntermediateResult,
	};

	static Result parse(char data, ParserState& state);
};

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
		qDebug() << "111";
	}

	return result;
}

class CommandParser : public QObject
{
	Q_OBJECT
private:
	bool testCommand(const char* command);
private slots:
	void initTestCase();
	void test();
};

bool CommandParser::testCommand(const char* command)
{
	ParserState state;
	int len = strlen(command);
	Parser::Result result;
	for (int i = 0; i < len; ++i) {
		result = Parser::parse(command[i], state);
		if (result != Parser::IntermediateResult) { break; }
	}
	return result == Parser::SuccessResult;
}

void CommandParser::initTestCase()
{

}

void CommandParser::test()
{
	QVERIFY(testCommand("LOG DUMP\r"));
	QVERIFY(testCommand("LOG DIR\r"));
	QVERIFY(testCommand("LOG PAGE\r"));
	QVERIFY(!testCommand("LOG OTHER\r"));
	QVERIFY(!testCommand("GET param\r"));
	QVERIFY(!testCommand("GER\r"));
}

QTEST_MAIN(CommandParser)

#include "command_parser.moc"
