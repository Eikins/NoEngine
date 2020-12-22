#include <fstream>

#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VKRenderDevice.h"
#include "Graphics/Vulkan/VKFramebuffer.h"

namespace Graphics
{

	void VKFramebuffer::Release()
	{
		VkDevice device = static_cast<VKRenderDevice*>(_device)->GetVulkanDevice();
		vkDestroyFramebuffer(device, _handle, nullptr);
	}

	void* VKFramebuffer::GetNativeHandle() const
	{
		return &_handle;
	}

}