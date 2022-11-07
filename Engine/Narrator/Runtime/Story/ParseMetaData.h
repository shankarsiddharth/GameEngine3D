#pragma once

#include <string>

#include "ParseMessageType.h"

namespace Narrator
{
	namespace Parser
	{
		struct ParseMetaData
		{
			ParseMetaData() :
				ParseMessageType(Narrator::Parser::TParseMessageType::kInfo),
				LineNumber(0)
			{
			}

			ParseMetaData(Narrator::Parser::TParseMessageType i_MessageType,
				std::size_t i_LineNumber, std::string i_Message) :
				ParseMessageType(i_MessageType),
				LineNumber(i_LineNumber),
				Message(i_Message)
			{
			}

			TParseMessageType ParseMessageType;
			std::size_t LineNumber;
			std::string Message;
		};
	}
}