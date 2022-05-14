#include <QtTest/QTest>
#include <QDebug>

#include <functional>

#include "parser.h"

class CommandParser : public QObject
{
	Q_OBJECT
private:
	bool testCommand(const char* command);

	void handler_logDir(CommandType, size_t paramCount, const char* params);
	void handler_logDump(CommandType, size_t paramCount, const char* params);
	void handler_logPage(CommandType, size_t paramCount, const char* params);
	void handler_get(CommandType, size_t paramCount, const char* params);
	void handler_set(CommandType, size_t paramCount, const char* params);
private slots:
	void initTestCase();
	void test();
};

bool CommandParser::testCommand(const char* command)
{
	ParserState state;
	size_t len = strlen(command);
	Parser::Result result;
	for (size_t i = 0; i < len; ++i) {
		result = Parser::parse(command[i], state);
		if (result != Parser::IntermediateResult) { break; }
	}
	return result == Parser::SuccessResult;
}

void CommandParser::handler_logDir(CommandType, size_t paramCount, const char*)
{
	qDebug() << "log_dir";
	QVERIFY(paramCount == 0);
}

void CommandParser::handler_logDump(CommandType, size_t paramCount, const char*)
{
	qDebug() << "log_dump";
	QVERIFY(paramCount == 0);
}

void CommandParser::handler_logPage(CommandType, size_t paramCount, const char*)
{
	qDebug() << "log_page";
	QVERIFY(paramCount == 0);
}

void CommandParser::handler_get(CommandType, size_t paramCount, const char* params)
{
	qDebug() << "get";
	QVERIFY(paramCount == 1);
	QVERIFY(params);
	QVERIFY(QString(Parser::parameter(0, params)) == "param");
}

void CommandParser::handler_set(CommandType, size_t paramCount, const char* params)
{
	qDebug() << "set";
	QVERIFY(paramCount == 2);
	QVERIFY(params);
	QVERIFY(QString(Parser::parameter(0, params)) == "param");
	QVERIFY(QString(Parser::parameter(1, params)) == "rrr");
}

void CommandParser::initTestCase()
{
	Parser::registerHandler(LogDirCommand, [this] (CommandType t, size_t c, const char* p) { handler_logDir(t, c, p); } );
	Parser::registerHandler(LogDumpCommand, [this] (CommandType t, size_t c, const char* p) { handler_logDump(t, c, p); } );
	Parser::registerHandler(LogPageCommand, [this] (CommandType t, size_t c, const char* p) { handler_logPage(t, c, p); } );
	Parser::registerHandler(GetCommand, [this] (CommandType t, size_t c, const char* p) { handler_get(t, c, p); } );
	Parser::registerHandler(SetCommand, [this] (CommandType t, size_t c, const char* p) { handler_set(t, c, p); } );
}

void CommandParser::test()
{
	QVERIFY(testCommand("LOG DUMP\r"));
	QVERIFY(testCommand("LOG DIR\r"));
	QVERIFY(testCommand("LOG PAGE\r"));
	QVERIFY(testCommand("GET param\r"));
	QVERIFY(!testCommand("LOG OTHER\r"));
	QVERIFY(!testCommand("GER\r"));
	QVERIFY(testCommand("SET param rrr \r"));
	QVERIFY(testCommand("SET param rrr\r"));
}

QTEST_MAIN(CommandParser)

#include "command_parser.moc"
