#include <QtTest/QTest>
#include <QDebug>

#include <functional>

#include "parser.h"

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
