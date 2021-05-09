#include "Graphics/Vulkan/VulkanSwapchain.h"

#include <stdexcept>

namespace Graphics
{
	void VulkanSwapchain::Init(VulkanContext* context, uint32_t width, uint32_t height)
	{
		_context = context;
		Recreate(width, height);
	}

	void VulkanSwapchain::Release()
	{
		if (_swapchain != VK_NULL_HANDLE)
		{
			for (uint32_t i = 0; i < _imageViews.size(); i++)
			{
				vkDestroyImageView(_context->device, _imageViews[i], nullptr);
			}
			vkDestroySwapchainKHR(_context->device, _swapchain, nullptr);
		}
	}

	void VulkanSwapchain::Recreate(uint32_t width, uint32_t height)
	{
		auto oldSwapchain = _swapchain;

		auto swapchainSupportDetails = _context->QuerySwapchainSupport();
		auto surfaceFormat = ChooseSurfaceFormat(swapchainSupportDetails.formats);
		auto presentMode = ChoosePresentMode(swapchainSupportDetails.presentModes);
		extent = ChooseExtent(swapchainSupportDetails.capabilities, width, height);
		format = surfaceFormat.format;

		// Sticking to the minimum may makes us wait on the driver to complete internal operations before we can aquire a new frame.
		uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
		uint32_t maxImageCount = swapchainSupportDetails.capabilities.maxImageCount;

		// Create the swapchain
		{
			if (maxImageCount > 0 && imageCount > maxImageCount)
			{
				imageCount = maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = _context->surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = _context->queueFamilies;
			uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			if (indices.graphicsFamily != indices.presentFamily)
			{
				// If both queue are separate
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0;
				createInfo.pQueueFamilyIndices = nullptr;
			}

			createInfo.preTransform = swapchainSupportDetails.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;
			createInfo.oldSwapchain = oldSwapchain;

			if (vkCreateSwapchainKHR(_context->device, &createInfo, nullptr, &_swapchain) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Swapchain!");
			}
		}

		// Release old image views and swapchain
		{
			if (oldSwapchain != VK_NULL_HANDLE)
			{
				for (uint32_t i = 0; i < imageCount; i++)
				{
					vkDestroyImageView(_context->device, _imageViews[i], nullptr);
				}
				vkDestroySwapchainKHR(_context->device, oldSwapchain, nullptr);
			}
		}

		// Retrieve swapchain images
		{
			vkGetSwapchainImagesKHR(_context->device, _swapchain, &imageCount, nullptr);
			_images.resize(imageCount);
			vkGetSwapchainImagesKHR(_context->device, _swapchain, &imageCount, _images.data());
		}

		// Create image views
		{
			_imageViews.resize(_images.size());

			for (size_t i = 0; i < _images.size(); i++)
			{
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = _images[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = format;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(_context->device, &createInfo, nullptr, &_imageViews[i]) != VK_SUCCESS)
				{
					throw std::runtime_error("Failed to create an image view!");
				}
			}
		}
	}

	VkResult VulkanSwapchain::AcquireImage(VkSemaphore imageAvailableSemaphore, uint32_t* imageIndex)
	{
		return vkAcquireNextImageKHR(_context->device, _swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, imageIndex);
	}

	VkResult VulkanSwapchain::PresentToScreen(uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &_swapchain;
		presentInfo.pImageIndices = &imageIndex;
		if (waitSemaphore != VK_NULL_HANDLE)
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &waitSemaphore;
		}
		return vkQueuePresentKHR(_context->presentQueue, &presentInfo);
	}

	uint32_t VulkanSwapchain::GetImageCount()
	{
		return _images.size();
	}

	VkImageView VulkanSwapchain::GetImageView(uint32_t index)
	{
		return _imageViews[index];
	}
#pragma region Internal Methods
	VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		// Find sRGB format here
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { width, height };
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	}
#pragma endregion
}