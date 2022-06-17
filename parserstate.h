#ifndef PARSERSTATE_H
#define PARSERSTATE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "parserdefines.h"

class ParserState
{
public:
	ParserState();

	void clear();

	inline int index() const { return m_index; }
	inline void setIndex(int index) { m_index = index; }
	inline void increaseIndex() { ++m_index; }

	inline int parameterIndex() const { return m_parameterIndex; }
	inline void setParameterIndex(int index) { m_parameterIndex = index; }

	inline size_t parameterCount() const { return m_parameterCount; }
	inline void setParameterCount(size_t count) { m_parameterCount = count; }

	inline CommandType commandType() const { return m_commandType; }
	inline void setCommandType(CommandType type) { m_commandType = type; }

	inline int commandIndex() const { return static_cast<int>(m_commandType); }
	inline void setCommandIndex(int index) { m_commandType = static_cast<CommandType>(index); }

	inline TokenType expectedToken() const { return m_expectedToken; }
	inline void setExpectedToken(TokenType type) { m_expectedToken = type; }

	bool appendParameterChar(char data);
	inline void nextParam() { ++m_parameterCount; ++m_parameterIndex; }

	inline const char* parameters() const { return &m_params[0]; }

	void registerHandler(CommandType commandType, CommandHandler&& handler);
	void resetHandlers();
	bool process() const;
private:
	CommandHandler handlers[CommandCount];
	int m_index = 0;
	int m_parameterIndex = -1;
	size_t m_parameterCount = 0;
	CommandType m_commandType = GetCommand;
	TokenType m_expectedToken = CommandToken;
	char m_params[MaxCommandSize];
};

#endif // PARSERSTATE_H
