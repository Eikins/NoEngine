#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VKSwapChain.h"

#include "Graphics/Vulkan/VKRenderDevice.h"

namespace Graphics
{
	void VKSwapChain::CreateImageViews()
	{
		VkDevice device = static_cast<VKRenderDevice*>(_device)->GetVulkanDevice();
		_imageViews.resize(_images.size());
		for (size_t i = 0; i < _images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = _images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = _imageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createInfo, nullptr, &_imageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create an image view!");
			}
		}
	}

	void VKSwapChain::Release()
	{
		VkDevice device = static_cast<VKRenderDevice*>(_device)->GetVulkanDevice();
		for (auto imageView : _imageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
		vkDestroySwapchainKHR(device, _handle, nullptr);
	}

	void* VKSwapChain::GetNativeHandle()
	{
		return &_handle;
	}
}