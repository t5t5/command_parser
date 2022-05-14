#ifndef PARSERSTATE_H
#define PARSERSTATE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "parserdefines.h"

class ParserState
{
public:
	ParserState() = default;

	inline void clear()
	{
		m_index = 0;
		m_parameterIndex = -1;
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
		m_params[m_parameterIndex + 1] = 0;
		++m_parameterIndex;
		return true;
	}
	inline void nextParam() { ++m_parameterCount; ++m_parameterIndex; }

	inline const char* parameters() const { return &m_params[0]; }
private:
	int m_index = 0;
	int m_parameterIndex = -1;
	int m_parameterCount = 0;
	CommandType m_commandType = GetCommand;
	TokenType m_expectedToken = CommandToken;
	char m_params[MaxCommandSize];
};

#endif // PARSERSTATE_H
