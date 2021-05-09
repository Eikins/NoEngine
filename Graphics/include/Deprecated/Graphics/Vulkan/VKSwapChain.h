#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/RenderPass.h"
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
		std::vector<VkFramebuffer> _framebuffers;
		VkFormat _imageFormat;
		VkExtent2D _extent;

		void CreateImageViews();
	public:
		void CreateFramebuffers(const RenderPass* renderPass);

		virtual void Release() override;
		virtual void* GetNativeHandle() const override;
	};
}