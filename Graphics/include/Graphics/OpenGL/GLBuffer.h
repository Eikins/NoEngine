#pragma once

#include "../Buffer.h"

namespace Graphics
{
	class GLBuffer : public Buffer
	{
	public:
		uint32_t GLID;

		virtual uint32_t GetNativeHandle() override;
	};
}