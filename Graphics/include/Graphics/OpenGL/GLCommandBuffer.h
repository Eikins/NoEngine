#pragma once

#include "Graphics/CommandBuffer.h"

namespace Graphics
{
	class GLCommandBuffer : public CommandBuffer
	{
	public:
		virtual void Clear(bool clearColor, bool clearDepth, float r, float g, float b, float a, float depth) override;
		virtual uint32_t GetNativeHandle() override;
	};
}