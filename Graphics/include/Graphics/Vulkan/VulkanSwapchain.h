#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanContext.h"

namespace Graphics
{
	class VulkanSwapchain
	{
	private:
		VulkanContext* _context;

		VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> _images;
		std::vector<VkImageView> _imageViews;

		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
	public:
		VkFormat format;
		VkExtent2D extent;

		void Init(VulkanContext* context, uint32_t width, uint32_t height);
		void Recreate(uint32_t width, uint32_t height);

		VkResult AcquireImage(VkSemaphore imageAvailableSemaphore, uint32_t* imageIndex);
		VkResult PresentToScreen(uint32_t imageIndex, VkSemaphore waitSemaphore);

		uint32_t GetImageCount();
		VkImageView GetImageView(uint32_t index);
	
		void Release();
	};
}