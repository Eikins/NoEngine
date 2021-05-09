#pragma once

#include <vulkan/vulkan.h>

namespace Graphics
{
	class VulkanContext;

	class VulkanBuffer
	{
	friend VulkanContext;
	private:
		VkDevice _device;
	public:
		VkBuffer buffer;
		VkDeviceMemory deviceMemory;
		void* mappedMemory = nullptr;

		void Bind();
		VkResult Map(VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);
		void Unmap();

		VkResult Flush(VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

		void Release();
	};
}