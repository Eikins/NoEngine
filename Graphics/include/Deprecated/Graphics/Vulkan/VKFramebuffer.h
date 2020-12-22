#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/Framebuffer.h"

namespace Graphics
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VKRenderDevice;
	class VKFramebuffer : public Framebuffer
	{
		friend VKRenderDevice;
	private:
		VkFramebuffer _handle;
		std::vector<VkImageView> _attachments;

	public:
		virtual void Release() override;
		virtual void* GetNativeHandle() const override;
	};
}