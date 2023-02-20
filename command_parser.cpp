#include <QtTest/QTest>
#include <QDebug>

#include <functional>

#include "parser.h"

class CommandParser : public QObject
{
	Q_OBJECT
private:
	bool testCommand(const char* command);

	bool handler_logDir(CommandType, size_t paramCount, const char* params);
	bool handler_logDump(CommandType, size_t paramCount, const char* params);
	bool handler_logPage(CommandType, size_t paramCount, const char* params);
	bool handler_get(CommandType, size_t paramCount, const char* params);
	bool handler_set(CommandType, size_t paramCount, const char* params);
	bool handler_gkhz(CommandType, size_t paramCount, const char* params);
private slots:
	void initTestCase();
	void test();
	void test_2();
	void test_3();
private:
	ParserState state;
};

bool CommandParser::testCommand(const char* command)
{
	size_t len = strlen(command);
	Parser::Result result;
	for (size_t i = 0; i < len; ++i) {
		result = Parser::parse(command[i], state);
//		if (result != Parser::IntermediateResult) { break; }
		if (result == Parser::IntermediateResult) {
			continue;
		} else
		if (result == Parser::FailResult) {
			// хз что тут делать
		} else
		if (result == Parser::SuccessResult) {
			// Если сюда зашли - значит обработчик уже вызвался, и команда должны быть выполнена
		}
		state.clear();
	}
	return result == Parser::SuccessResult;
}

bool CommandParser::handler_logDir(CommandType, size_t paramCount, const char*)
{
	bool result = paramCount == 0;
	qDebug() << "log_dir " << result;
	return result;
}

bool CommandParser::handler_logDump(CommandType, size_t paramCount, const char*)
{
	bool result = paramCount == 0;
	qDebug() << "log_dump " << result;
	return result;
}

bool CommandParser::handler_logPage(CommandType, size_t paramCount, const char*)
{
	bool result = paramCount == 0;
	qDebug() << "log_page " << result;
	return result;
}

bool CommandParser::handler_get(CommandType, size_t paramCount, const char* params)
{
	bool result =
		(paramCount == 1) &&
		(params) &&
		((QString(Parser::parameter(0, params)) == "param") ||
		 (QString(Parser::parameter(0, params)) ==  "UFM_FLOWSPEED"));
	qDebug() << "get " << result;
	return result;
}

bool CommandParser::handler_set(CommandType, size_t paramCount, const char* params)
{
	bool result =
		(paramCount == 2) &&
		(params) &&
		(QString(Parser::parameter(0, params)) == "param") &&
		(QString(Parser::parameter(1, params)) == "rrr");
	qDebug() << "set " << result;
	return result;
}

bool CommandParser::handler_gkhz(CommandType, size_t paramCount, const char* params)
{
	bool result = paramCount == 0;
	qDebug() << "gkhz " << result;
	return result;
}

void CommandParser::initTestCase()
{
	state.registerHandler(LogDirCommand, [this] (CommandType t, size_t c, const char* p) { return handler_logDir(t, c, p); } );
	state.registerHandler(LogDumpCommand, [this] (CommandType t, size_t c, const char* p) { return handler_logDump(t, c, p); } );
	state.registerHandler(LogPageCommand, [this] (CommandType t, size_t c, const char* p) { return handler_logPage(t, c, p); } );
	state.registerHandler(GetCommand, [this] (CommandType t, size_t c, const char* p) { return handler_get(t, c, p); } );
	state.registerHandler(SetCommand, [this] (CommandType t, size_t c, const char* p) { return handler_set(t, c, p); } );
	state.registerHandler(GkhzCommand, [this] (CommandType t, size_t c, const char* p) { return handler_gkhz(t, c, p); } );
}

void CommandParser::test()
{
	bool state = testCommand("\r\n\r\r\n\n\n\r");
//	QVERIFY(state);
	QVERIFY(testCommand("r\r\r\n\n\n\r\nGET UFM_FLOWSPEED\n"));
	QVERIFY(testCommand("GET UFM_FLOWSPEED\r"));
	QVERIFY(testCommand("GET UFM_FLOWSPEED\n"));
	QVERIFY(testCommand("GET UFM_FLOWSPEED\r"));
	QVERIFY(testCommand("LOG DUMP\r"));
	QVERIFY(testCommand("LOG DIR\r"));
	QVERIFY(testCommand("LOG PAGE\r"));
	QVERIFY(!testCommand("GET parm\r"));
	QVERIFY(testCommand("GET param\r"));
	QVERIFY(!testCommand("LOG OTHER\r"));
	QVERIFY(!testCommand("GER\r"));
	QVERIFY(testCommand("SET param rrr \r"));
	QVERIFY(testCommand("SET param rrr\r"));
}

void CommandParser::test_2()
{
	qDebug() << "----test_2----";
	testCommand("polnayaerunda\rLOG DUMP\rSET param rrr\rGET\rtuttoje hren\r");
	qDebug() << "----test_2----";
}

void CommandParser::test_3()
{
	qDebug() << "----test_3----";
	QVERIFY(!testCommand("G"));
	QVERIFY(!testCommand("K"));
	QVERIFY(!testCommand("H"));
	QVERIFY(!testCommand("Z"));
	QVERIFY(testCommand("\r"));
	qDebug() << "----test_3----";
}

QTEST_MAIN(CommandParser)

#include "command_parser.moc"
