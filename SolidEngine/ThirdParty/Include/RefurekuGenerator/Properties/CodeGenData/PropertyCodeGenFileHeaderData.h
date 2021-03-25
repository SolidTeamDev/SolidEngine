/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "RefurekuGenerator/Properties/CodeGenData/PropertyCodeGenData.h"

namespace rfk
{
	class PropertyCodeGenFileHeaderData : public PropertyCodeGenData
	{
		//No additional data for now

		public:
			PropertyCodeGenFileHeaderData()										noexcept;
			PropertyCodeGenFileHeaderData(PropertyCodeGenFileHeaderData const&) = default;
			PropertyCodeGenFileHeaderData(PropertyCodeGenFileHeaderData&&)		= default;
			~PropertyCodeGenFileHeaderData()									= default;
	};
}