#pragma once

#include <vulkan/vulkan.h>

namespace Graphics
{
	namespace VkCreate
	{
		inline VkViewport Viewport(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
		{
			VkViewport viewport{};
			viewport.x = x;
			viewport.y = y;
			viewport.width = width;
			viewport.height = height;
			viewport.minDepth = minDepth;
			viewport.maxDepth = maxDepth;
			return viewport;
		}

		inline VkRect2D Rect2D(int x, int y, int width, int height)
		{
			VkRect2D rect2D{};
			rect2D.offset.x = x;
			rect2D.offset.y = y;
			rect2D.extent.width = width;
			rect2D.extent.height = height;
			return rect2D;
		}

		inline VkBufferCreateInfo bufferCreateInfo(VkBufferUsageFlags usage, VkDeviceSize size)
		{
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.usage = usage;
			bufferCreateInfo.size = size;
			return bufferCreateInfo;
		}

	}
}