#include <fstream>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VKRenderDevice.h"
#include "Graphics/Vulkan/VKPipelineState.h"

namespace Graphics
{

	void VKPipelineState::Release()
	{
		VkDevice device = static_cast<VKRenderDevice*>(_device)->GetVulkanDevice();
		vkDestroyPipeline(device, _pipeline, nullptr);
		vkDestroyPipelineLayout(device, _layout, nullptr);
		vkDestroyRenderPass(device, _renderPass, nullptr);
	}

	void* VKPipelineState::GetNativeHandle()
	{
		return nullptr;
	}
}