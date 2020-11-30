#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/SwapChain.h"

namespace Graphics
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VKRenderDevice;
	class VKSwapChain : public SwapChain
	{
		friend VKRenderDevice;
	private:
		VkSwapchainKHR _handle;
		std::vector<VkImage> _images;
		std::vector<VkImageView> _imageViews;
		VkFormat _imageFormat;
		VkExtent2D _extent;

		void CreateImageViews();
	public:
		virtual void Release() override;
		virtual void* GetNativeHandle() override;
	};
}