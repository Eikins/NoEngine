#pragma once

#include <memory>

namespace Graphics
{
	class CommandBuffer
	{
	public:
		virtual void Clear(bool clearColor, bool clearDepth, float r, float g, float b, float a, float depth) = 0;
		virtual uint32_t GetNativeHandle() = 0;
	};
}