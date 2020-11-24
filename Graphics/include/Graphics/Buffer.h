#pragma once

#include <memory>

namespace Graphics
{
	struct BufferDescriptor
	{
		uint32_t size = 0;
		uint32_t stride = 0;
	};

	struct BufferData
	{
		void* data;
		uint32_t size;
	};

	class Buffer
	{
	public:
		virtual uint32_t GetNativeHandle() = 0;
	};
}