#include "Graphics/Vulkan/VulkanBuffer.h"

namespace Graphics
{
	void VulkanBuffer::Bind()
	{
		vkBindBufferMemory(_device, buffer, deviceMemory, 0);
	}

	VkResult VulkanBuffer::Map(VkDeviceSize offset, VkDeviceSize size)
	{
		return vkMapMemory(_device, deviceMemory, offset, size, 0, &mappedMemory);
	}

	void VulkanBuffer::Unmap()
	{
		if (mappedMemory != nullptr)
		{
			vkUnmapMemory(_device, deviceMemory);
			mappedMemory = nullptr;
		}
	}

	VkResult VulkanBuffer::Flush(VkDeviceSize offset, VkDeviceSize size)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = deviceMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
	}

	void VulkanBuffer::Release()
	{
		Unmap();
		if (buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(_device, buffer, nullptr);
		}
		if (deviceMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(_device, deviceMemory, nullptr);
		}
	}
}