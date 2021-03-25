/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

inline EnumValueParsingResult* EnumValueParser::getParsingResult() noexcept
{
	return reinterpret_cast<EnumValueParsingResult*>(getContext().parsingResult);
}