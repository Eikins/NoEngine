/*
* This file holds a copy of initializes by Sascha Willems
* https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanInitializers.hpp
* 
* File Copyright :
* Initializers for Vulkan structures and objects used by the examples
* Saves lot of VK_STRUCTURE_TYPE assignments
* Some initializers are parameterized for convenience
* 
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <vulkan/vulkan.h>

namespace Graphics
{
	namespace VkCreate
	{
		inline VkViewport Viewport(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
		{
			VkViewport viewport{};
			viewport.x = x;
			viewport.y = y;
			viewport.width = width;
			viewport.height = height;
			viewport.minDepth = minDepth;
			viewport.maxDepth = maxDepth;
			return viewport;
		}

		inline VkRect2D Rect2D(int x, int y, int width, int height)
		{
			VkRect2D rect2D{};
			rect2D.offset.x = x;
			rect2D.offset.y = y;
			rect2D.extent.width = width;
			rect2D.extent.height = height;
			return rect2D;
		}

		inline VkPushConstantRange PushConstantRange(VkShaderStageFlags stageFlags, uint32_t size, uint32_t offset)
		{
			VkPushConstantRange pushConstantRange{};
			pushConstantRange.stageFlags = stageFlags;
			pushConstantRange.offset = offset;
			pushConstantRange.size = size;
			return pushConstantRange;
		}

		inline VkBufferCopy BufferCopyRegion(uint32_t offset, uint32_t size)
		{
			VkBufferCopy copyRegion{};
			copyRegion.srcOffset = offset;
			copyRegion.dstOffset = offset;
			copyRegion.size = size;
			return copyRegion;
		}

		inline VkDescriptorBufferInfo DescriptorBufferInfo(VkBuffer buffer, uint32_t offset, uint32_t range)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = buffer;
			bufferInfo.offset = offset;
			bufferInfo.range = range;
			return bufferInfo;
		}

		inline VkVertexInputBindingDescription VertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
		{
			VkVertexInputBindingDescription vInputBindDescription{};
			vInputBindDescription.binding = binding;
			vInputBindDescription.stride = stride;
			vInputBindDescription.inputRate = inputRate;
			return vInputBindDescription;
		}

		inline VkVertexInputAttributeDescription VertexInputAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
		{
			VkVertexInputAttributeDescription vInputAttribDescription{};
			vInputAttribDescription.location = location;
			vInputAttribDescription.binding = binding;
			vInputAttribDescription.format = format;
			vInputAttribDescription.offset = offset;
			return vInputAttribDescription;
		}

		inline VkBufferCreateInfo BufferCreateInfo(VkBufferUsageFlags usage, VkDeviceSize size)
		{
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.usage = usage;
			bufferCreateInfo.size = size;
			return bufferCreateInfo;
		}

		inline VkImageCreateInfo ImageCreateInfo()
		{
			VkImageCreateInfo imageCreateInfo{};
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			return imageCreateInfo;
		}

		inline VkMemoryAllocateInfo MemoryAllocateInfo()
		{
			VkMemoryAllocateInfo memAllocInfo{};
			memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			return memAllocInfo;
		}

		inline VkSamplerCreateInfo SamplerCreateInfo(
			VkFilter filter = VK_FILTER_LINEAR,
			VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR, 
			VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
			float maxAnisotropy = 1.0f, 
			float minLod = -1000.0f, 
			float maxLod = 1000.0f
		)
		{
			VkSamplerCreateInfo samplerCreateInfo = {};
			samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerCreateInfo.magFilter = filter;
			samplerCreateInfo.minFilter = filter;
			samplerCreateInfo.mipmapMode = mipmapMode;
			samplerCreateInfo.addressModeU = addressMode;
			samplerCreateInfo.addressModeV = addressMode;
			samplerCreateInfo.addressModeW = addressMode;
			samplerCreateInfo.maxAnisotropy = maxAnisotropy;
			samplerCreateInfo.minLod = minLod;
			samplerCreateInfo.maxLod = maxLod;
			return samplerCreateInfo;
		}

		inline VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo(uint32_t count, VkDescriptorPoolSize* poolSizes, uint32_t maxSets)
		{
			VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
			descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			descriptorPoolCreateInfo.poolSizeCount = count;
			descriptorPoolCreateInfo.pPoolSizes = poolSizes;
			descriptorPoolCreateInfo.maxSets = maxSets;
			return descriptorPoolCreateInfo;
		}

		inline VkDescriptorPoolSize DescriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount)
		{
			VkDescriptorPoolSize descriptorPoolSize{};
			descriptorPoolSize.type = type;
			descriptorPoolSize.descriptorCount = descriptorCount;
			return descriptorPoolSize;
		}

		inline VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding, uint32_t descriptorCount = 1)
		{
			VkDescriptorSetLayoutBinding setLayoutBinding{};
			setLayoutBinding.descriptorType = type;
			setLayoutBinding.stageFlags = stageFlags;
			setLayoutBinding.binding = binding;
			setLayoutBinding.descriptorCount = descriptorCount;
			return setLayoutBinding;
		}

		inline VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* pBindings, uint32_t bindingCount)
		{
			VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
			descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutCreateInfo.pBindings = pBindings;
			descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
			return descriptorSetLayoutCreateInfo;
		}

		inline VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo(VkDescriptorPool descriptorPool, const VkDescriptorSetLayout* setLayouts, uint32_t descriptorSetCount)
		{
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.descriptorPool = descriptorPool;
			descriptorSetAllocateInfo.pSetLayouts = setLayouts;
			descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
			return descriptorSetAllocateInfo;
		}

		inline VkDescriptorImageInfo DescriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout)
		{
			VkDescriptorImageInfo descriptorImageInfo{};
			descriptorImageInfo.sampler = sampler;
			descriptorImageInfo.imageView = imageView;
			descriptorImageInfo.imageLayout = imageLayout;
			return descriptorImageInfo;
		}

		inline VkWriteDescriptorSet WriteDescriptorSet(VkDescriptorSet dstSet, VkDescriptorType type, uint32_t binding, VkDescriptorImageInfo* imageInfo, uint32_t descriptorCount = 1)
		{
			VkWriteDescriptorSet writeDescriptorSet{};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.dstSet = dstSet;
			writeDescriptorSet.descriptorType = type;
			writeDescriptorSet.dstBinding = binding;
			writeDescriptorSet.pImageInfo = imageInfo;
			writeDescriptorSet.descriptorCount = descriptorCount;
			return writeDescriptorSet;
		}

		inline VkShaderModuleCreateInfo ShaderModuleCreateInfo(size_t size, const uint32_t* code)
		{
			VkShaderModuleCreateInfo shaderModuleCreateInfo{};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = size;
			shaderModuleCreateInfo.pCode = code;
			return shaderModuleCreateInfo;
		}

		inline VkPipelineCacheCreateInfo PipelineCacheCreateInfo()
		{
			VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
			pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			return pipelineCacheCreateInfo;
		}

		inline VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo(const VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount = 1)
		{
			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
			pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
			return pipelineLayoutCreateInfo;
		}

		inline VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkPipelineInputAssemblyStateCreateFlags flags, VkBool32 primitiveRestartEnable)
		{
			VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
			pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			pipelineInputAssemblyStateCreateInfo.topology = topology;
			pipelineInputAssemblyStateCreateInfo.flags = flags;
			pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
			return pipelineInputAssemblyStateCreateInfo;
		}

		inline VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo()
		{
			VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{};
			pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			return pipelineVertexInputStateCreateInfo;
		}

		inline VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, VkPipelineRasterizationStateCreateFlags flags = 0)
		{
			VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
			pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
			pipelineRasterizationStateCreateInfo.cullMode = cullMode;
			pipelineRasterizationStateCreateInfo.frontFace = frontFace;
			pipelineRasterizationStateCreateInfo.flags = flags;
			pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
			return pipelineRasterizationStateCreateInfo;
		}

		inline VkPipelineColorBlendAttachmentState PipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable)
		{
			VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{};
			pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
			pipelineColorBlendAttachmentState.blendEnable = blendEnable;
			return pipelineColorBlendAttachmentState;
		}

		inline VkPipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo(uint32_t attachmentCount, const VkPipelineColorBlendAttachmentState* pAttachments)
		{
			VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
			pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
			pipelineColorBlendStateCreateInfo.pAttachments = pAttachments;
			return pipelineColorBlendStateCreateInfo;
		}

		inline VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp)
		{
			VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo{};
			pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
			pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
			pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
			pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
			return pipelineDepthStencilStateCreateInfo;
		}

		inline VkPipelineViewportStateCreateInfo PipelineViewportStateCreateInfo(uint32_t viewportCount, uint32_t scissorCount, VkPipelineViewportStateCreateFlags flags = 0)
		{
			VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
			pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			pipelineViewportStateCreateInfo.viewportCount = viewportCount;
			pipelineViewportStateCreateInfo.scissorCount = scissorCount;
			pipelineViewportStateCreateInfo.flags = flags;
			return pipelineViewportStateCreateInfo;
		}

		inline VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples, VkPipelineMultisampleStateCreateFlags flags = 0)
		{
			VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
			pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
			pipelineMultisampleStateCreateInfo.flags = flags;
			return pipelineMultisampleStateCreateInfo;
		}

		inline VkPipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo(const VkDynamicState* pDynamicStates, uint32_t dynamicStateCount, VkPipelineDynamicStateCreateFlags flags = 0)
		{
			VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
			pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates;
			pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
			pipelineDynamicStateCreateInfo.flags = flags;
			return pipelineDynamicStateCreateInfo;
		}

		inline VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* name, VkShaderModule module)
		{
			VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.stage = stage;
			shaderStageCreateInfo.pName = name;
			shaderStageCreateInfo.module = module;
			return shaderStageCreateInfo;
		}

		inline VkGraphicsPipelineCreateInfo PipelineCreateInfo(VkPipelineLayout layout, VkRenderPass renderPass, VkPipelineCreateFlags flags = 0)
		{
			VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.layout = layout;
			pipelineCreateInfo.renderPass = renderPass;
			pipelineCreateInfo.flags = flags;
			pipelineCreateInfo.basePipelineIndex = -1;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			return pipelineCreateInfo;
		}
	}
}