#include "parserstate.h"

ParserState::ParserState()
{
	resetHandlers();
}

void ParserState::clear()
{
	m_index = 0;
	m_parameterIndex = -1;
	m_parameterCount = 0;
	m_commandType = GetCommand;
	m_expectedToken = CommandToken;
	std::memset(m_params, 0, MaxCommandSize);
}

bool ParserState::appendParameterChar(char data)
{
	if ((m_parameterIndex + 1) == MaxCommandSize) { return false; }
	m_params[m_parameterIndex + 0] = data;
	m_params[m_parameterIndex + 1] = 0;
	++m_parameterIndex;
	return true;
}

void ParserState::registerHandler(CommandType commandType, CommandHandler&& handler)
{
	int i = static_cast<int>(commandType);
	if (i >= CommandCount) { return; }

	handlers[i] = handler;
}

void ParserState::resetHandlers()
{
	for (int i = 0; i < CommandCount; ++i) { handlers[i] = nullptr; }
}

bool ParserState::process() const
{
	int i = static_cast<int>(m_commandType);
	if (i >= CommandCount) { return false; }

	auto handler = handlers[i];
	if (handler) { return handler(m_commandType, m_parameterCount, parameters()); }
	return false;
}
