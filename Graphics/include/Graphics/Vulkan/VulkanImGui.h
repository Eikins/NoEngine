#pragma once

#include "VulkanContext.h"
#include "VulkanBuffer.h"

#include "Math/Vector2.h"

namespace Graphics
{
	class VulkanImGui
	{
		// Class inspired from SaschaWillems C++ Vulkan Examples
		// https://github.com/SaschaWillems/Vulkan/blob/master/examples/imgui/main.cpp
	private:
		struct PushConstants
		{
			Math::Vector2 scale;
			Math::Vector2 translation;
		} _pushConstants;

		VulkanContext* _context;
		
		VulkanBuffer _vertexBuffer;
		VulkanBuffer _indexBuffer;
		uint32_t _vertexCount = 0;
		uint32_t _indexCount = 0;

		VkImage _fontImage = VK_NULL_HANDLE;
		VkDeviceMemory _fontMemory = VK_NULL_HANDLE;
		VkImageView _fontView = VK_NULL_HANDLE;
		VkSampler _fontSampler = VK_NULL_HANDLE;

		VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;

		VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
		VkPipeline _pipeline = VK_NULL_HANDLE;
	public:
		void Init(VulkanContext* context, float width, float height);
		void CreateResources(VkRenderPass renderPass, VkQueue copyQueue);

		void UpdateBuffers();
		void NewFrame();
		void RecordDrawCommands(VkCommandBuffer commandBuffer);

		void Release();
	};
}