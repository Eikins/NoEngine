#include "Graphics/Vulkan/VulkanMeshRenderer.h"

#include "Graphics/Vulkan/VulkanUtils.hpp"

namespace Graphics
{
	void VulkanMeshRenderer::SetupCameraProperties(Core::Camera& camera)
	{
		_constantBuffer.viewMatrix = camera.GetTransform()->GetLocalToWorldMatrix().InverseTR();
		_constantBuffer.projectionMatrix = camera.GetProjectionMatrix();
	}

	VulkanMeshRenderer::BakedMaterial* VulkanMeshRenderer::FindBakedMaterial(Core::Material* material)
	{
		for (int i = 0; i < _bakedMaterials.size(); i++)
		{
			if (_bakedMaterials[i].material == material) return &_bakedMaterials[i];
		}
		return nullptr;
	}

	VulkanMeshRenderer::BakedMesh* VulkanMeshRenderer::FindBakedMesh(Core::Mesh* mesh)
	{
		for (int i = 0; i < _bakedMeshes.size(); i++)
		{
			if (_bakedMeshes[i].mesh == mesh) return &_bakedMeshes[i];
		}
		return nullptr;
	}

	VulkanMeshRenderer::BakedRenderer* VulkanMeshRenderer::FindBakedRenderer(Core::Renderer* renderer)
	{
		for (int i = 0; i < _bakedRenderers.size(); i++)
		{
			if (_bakedRenderers[i].renderer == renderer) return &_bakedRenderers[i];
		}
		return nullptr;
	}

	VulkanMeshRenderer::BakedMaterial* VulkanMeshRenderer::BakeMaterial(Core::Material* material)
	{
		uint32_t index = _bakedMaterials.size();
		if (index >= _materialLimit)
		{
			throw std::runtime_error("Maximum material limit exceeded...");
		}
		_bakedMaterials.push_back(BakedMaterial());
		auto& bakedMaterial = _bakedMaterials[index];
		bakedMaterial.material = material;

		// Create Layouts
		{
			// No custom descriptor set support yet
			auto layoutBinding = VkCreate::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0, 1);
			auto descriptorSetLayoutCreateInfo = VkCreate::DescriptorSetLayoutCreateInfo(&layoutBinding, 1);

			if (vkCreateDescriptorSetLayout(_context->device, &descriptorSetLayoutCreateInfo, nullptr, &bakedMaterial.descriptorSetLayout) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create material descriptor set layout!");
			}

			auto pipelineLayoutCreateInfo = VkCreate::PipelineLayoutCreateInfo(&bakedMaterial.descriptorSetLayout);
			pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
			pipelineLayoutCreateInfo.pPushConstantRanges = &_pushConstantRange;

			if (vkCreatePipelineLayout(_context->device, &pipelineLayoutCreateInfo, nullptr, &bakedMaterial.pipelineLayout) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create material pipeline layout!");
			}
		}

		// Create Graphics Pipeline
		{
			VkVertexInputBindingDescription vertexInputBindings[] =
			{
				VkCreate::VertexInputBindingDescription(0, sizeof(Core::VertexData), VK_VERTEX_INPUT_RATE_VERTEX)
			};

			VkVertexInputAttributeDescription vertexInputAttributes[] =
			{
				VkCreate::VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Core::VertexData, position)),
				VkCreate::VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Core::VertexData, normal)),
				VkCreate::VertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Core::VertexData, tangent)),
				VkCreate::VertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(Core::VertexData, texCoords))
			};

			auto inputAssemblyState = VkCreate::PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
			auto vertexInputState = VkCreate::PipelineVertexInputStateCreateInfo();
			vertexInputState.vertexBindingDescriptionCount = 1;
			vertexInputState.vertexAttributeDescriptionCount = 4;
			vertexInputState.pVertexBindingDescriptions = vertexInputBindings;
			vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes;

			auto rasterizationState = VkCreate::PipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
			
			auto renderStateBlock = material->GetRenderStateBlock();
			VkPipelineColorBlendAttachmentState blendAttachmentState;
			if (renderStateBlock.blendEnabled)
			{
				blendAttachmentState = VkCreate::PipelineColorBlendAttachmentState(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, VK_TRUE);
				blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
			}
			else
			{
				blendAttachmentState = VkCreate::PipelineColorBlendAttachmentState(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, VK_FALSE);
			}

			auto colorBlendState = VkCreate::PipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
			auto depthStencilState = VkCreate::PipelineDepthStencilStateCreateInfo(
				renderStateBlock.depthTestEnabled ? VK_TRUE : VK_FALSE,
				renderStateBlock.depthWrite ? VK_TRUE : VK_FALSE,
				VK_COMPARE_OP_LESS_OR_EQUAL
			);
			auto viewportState = VkCreate::PipelineViewportStateCreateInfo(1, 1, 0);
			auto multisampleState = VkCreate::PipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);

			VkDynamicState dynamicStates[] =
			{
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			auto dynamicState = VkCreate::PipelineDynamicStateCreateInfo(dynamicStates, 2);

			// Create shader modules
			auto vertexShader = material->GetVertexShader();
			auto fragmentShader = material->GetFragmentShader();
			auto vertexCode = vertexShader->GetCompiledCode();
			auto fragmentCode = fragmentShader->GetCompiledCode();

			auto vertexShaderCreateInfo = VkCreate::ShaderModuleCreateInfo(vertexCode.size(), reinterpret_cast<const uint32_t*>(vertexCode.data()));
			auto fragmentShaderCreateInfo = VkCreate::ShaderModuleCreateInfo(fragmentCode.size(), reinterpret_cast<const uint32_t*>(fragmentCode.data()));
			
			VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
			VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;

			if (vkCreateShaderModule(_context->device, &vertexShaderCreateInfo, nullptr, &vertexShaderModule) != VK_SUCCESS)
			{
				throw std::runtime_error("Couldn't create ImGui vertex shader !");
			}

			if (vkCreateShaderModule(_context->device, &fragmentShaderCreateInfo, nullptr, &fragmentShaderModule) != VK_SUCCESS)
			{
				throw std::runtime_error("Couldn't create ImGui fragment shader !");
			}

			VkPipelineShaderStageCreateInfo shaderStages[] =
			{
				VkCreate::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main", vertexShaderModule),
				VkCreate::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragmentShaderModule)
			};

			auto pipelineCreateInfo = VkCreate::PipelineCreateInfo(bakedMaterial.pipelineLayout, _renderPass);
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
			pipelineCreateInfo.pVertexInputState = &vertexInputState;
			pipelineCreateInfo.pRasterizationState = &rasterizationState;
			pipelineCreateInfo.pColorBlendState = &colorBlendState;
			pipelineCreateInfo.pMultisampleState = &multisampleState;
			pipelineCreateInfo.pViewportState = &viewportState;
			pipelineCreateInfo.pDepthStencilState = &depthStencilState;
			pipelineCreateInfo.pDynamicState = &dynamicState;
			pipelineCreateInfo.stageCount = 2;
			pipelineCreateInfo.pStages = shaderStages;

			if (vkCreateGraphicsPipelines(_context->device, nullptr, 1, &pipelineCreateInfo, nullptr, &bakedMaterial.pipeline))
			{
				throw std::runtime_error("Couldn't create ImGui pipeline !");
			}

			vkDestroyShaderModule(_context->device, vertexShaderModule, nullptr);
			vkDestroyShaderModule(_context->device, fragmentShaderModule, nullptr);
		}

		return &bakedMaterial;
	}

	VulkanMeshRenderer::BakedMesh* VulkanMeshRenderer::BakeMesh(Core::Mesh* mesh)
	{
		uint32_t index = _bakedMeshes.size();
		if (index >= _meshLimit)
		{
			throw std::runtime_error("Maximum mesh limit exceeded...");
		}
		_bakedMeshes.push_back(BakedMesh());
		auto& bakedMesh = _bakedMeshes[index];
		bakedMesh.mesh = mesh;

		auto vertices = mesh->GenerateInterleavedData();
		auto indices = mesh->GetIndices();
		auto verticesByteSize = vertices.size() * sizeof(Core::VertexData);
		auto indicesByteSize = indices.size() * sizeof(uint16_t);

		bakedMesh.indexCount = indices.size();

		// Upload data to a staging buffers
		auto vertexStagingBuffer = _context->CreateStagingBuffer(verticesByteSize);
		vertexStagingBuffer.Bind();
		vertexStagingBuffer.Map();
		memcpy(vertexStagingBuffer.mappedMemory, vertices.data(), verticesByteSize);
		vertexStagingBuffer.Unmap();

		auto indexStagingBuffer = _context->CreateStagingBuffer(indicesByteSize);
		indexStagingBuffer.Bind();
		indexStagingBuffer.Map();
		memcpy(indexStagingBuffer.mappedMemory, indices.data(), indicesByteSize);
		indexStagingBuffer.Unmap();

		// Transfer to a device local buffer
		bakedMesh.vertices = _context->CreateBuffer(verticesByteSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		bakedMesh.indices = _context->CreateBuffer(indicesByteSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		bakedMesh.vertices.Bind();
		bakedMesh.indices.Bind();

		auto commandBuffer = _context->CreateCommandBuffer(true);
		auto vertexRegion = VkCreate::BufferCopyRegion(0, verticesByteSize);
		auto indexRegion = VkCreate::BufferCopyRegion(0, indicesByteSize);
		vkCmdCopyBuffer(commandBuffer, vertexStagingBuffer.buffer, bakedMesh.vertices.buffer, 1, &vertexRegion);
		vkCmdCopyBuffer(commandBuffer, indexStagingBuffer.buffer, bakedMesh.indices.buffer, 1, &indexRegion);
		_context->ExecuteCommandBuffer(commandBuffer);

		vertexStagingBuffer.Release();
		indexStagingBuffer.Release();

		return &bakedMesh;
	}

	VulkanMeshRenderer::BakedRenderer* VulkanMeshRenderer::BakeRenderer(Core::Renderer* renderer)
	{
		uint32_t index = _bakedRenderers.size();
		if (index >= _rendererLimit)
		{
			throw std::runtime_error("Maximum renderer limit exceeded...");
		}
		_bakedRenderers.push_back(BakedRenderer());
		auto& bakedRenderer = _bakedRenderers[index];
		bakedRenderer.renderer = renderer;
		auto bakedMaterial = FindBakedMaterial(renderer->material);
		if (bakedMaterial == nullptr)
		{
			bakedMaterial = BakeMaterial(renderer->material);
		}

		auto bakedMesh = FindBakedMesh(renderer->mesh);
		if (bakedMesh == nullptr)
		{
			bakedMesh = BakeMesh(renderer->mesh);
		}

		bakedRenderer.bakedMaterial = bakedMaterial;
		bakedRenderer.bakedMesh = bakedMesh;

		// Create Descriptor set
		auto descriptorSetAllocateInfo = VkCreate::DescriptorSetAllocateInfo(_descriptorPool, &bakedMaterial->descriptorSetLayout, 1);
		if (vkAllocateDescriptorSets(_context->device, &descriptorSetAllocateInfo, &bakedRenderer.descriptorSet) != VK_SUCCESS)
		{
			throw std::runtime_error("Couldn't create mesh descriptor set !");
		}

		// Create the uniform (Per Object) buffer
		// We don't use a device local buffer here because we will update these values every frame
		bakedRenderer.perObjectBuffer = _context->CreateBuffer(
			sizeof(MeshPerObjectBuffer),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		bakedRenderer.perObjectBuffer.Bind();
		bakedRenderer.perObjectBuffer.Map();

		// Bind buffer to the descriptor set
		auto bufferInfo = VkCreate::DescriptorBufferInfo(bakedRenderer.perObjectBuffer.buffer, 0, sizeof(MeshPerObjectBuffer));
		auto descriptorWrite = VkCreate::WriteDescriptorSet(bakedRenderer.descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, nullptr);
		descriptorWrite.pBufferInfo = &bufferInfo;
		vkUpdateDescriptorSets(_context->device, 1, &descriptorWrite, 0, nullptr);

		return &bakedRenderer;
	}

	void VulkanMeshRenderer::PrepareRenderers(const std::vector<Core::Renderer*>& renderers)
	{
		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = renderers[i];
			auto bakedRenderer = FindBakedRenderer(renderer);
			if (bakedRenderer == nullptr)
			{
				bakedRenderer = BakeRenderer(renderer);
			}
		}
	}

	void VulkanMeshRenderer::UpdateBuffers(const std::vector<Core::Renderer*>& renderers)
	{
		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = renderers[i];
			auto bakedRenderer = FindBakedRenderer(renderer);
			if (bakedRenderer != nullptr)
			{
				MeshPerObjectBuffer perObjectBufferData
				{
					renderer->GetTransform()->GetLocalToWorldMatrix()
				};

				memcpy(bakedRenderer->perObjectBuffer.mappedMemory, &perObjectBufferData, sizeof(perObjectBufferData));
				bakedRenderer->perObjectBuffer.Flush();
			}
		}
	}

	void VulkanMeshRenderer::RecordDrawCommands(VkCommandBuffer commandBuffer, const std::vector<Core::Renderer*>& renderers, float width, float height)
	{
		VkViewport viewport = VkCreate::Viewport(0, 0, width, height);
		auto scissor = VkCreate::Rect2D(0, 0, width, height);

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkDeviceSize offsets[] = { 0 };

		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = renderers[i];
			auto bakedRenderer = FindBakedRenderer(renderer);
			if (bakedRenderer != nullptr)
			{
				auto bakedMaterial = bakedRenderer->bakedMaterial;
				auto bakedMesh = bakedRenderer->bakedMesh;

				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bakedMaterial->pipeline);
				vkCmdPushConstants(commandBuffer, bakedMaterial->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ConstantBuffer), &_constantBuffer);
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bakedMaterial->pipelineLayout, 0, 1, &bakedRenderer->descriptorSet, 0, nullptr);
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, &bakedMesh->vertices.buffer, offsets);
				vkCmdBindIndexBuffer(commandBuffer, bakedMesh->indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdDrawIndexed(commandBuffer, bakedMesh->indexCount, 1, 0, 0, 0);
			}
		}
	}

	void VulkanMeshRenderer::Init(VulkanContext* ctx, VkRenderPass renderPass, uint32_t maximumMaterials, uint32_t maximumMeshes, uint32_t maximumRenderers)
	{
		_context = ctx;
		_materialLimit = maximumMaterials;
		_meshLimit = maximumMeshes;
		_rendererLimit = maximumRenderers;

		_bakedMaterials.reserve(maximumMaterials);
		_bakedMeshes.reserve(maximumMeshes);
		_bakedRenderers.reserve(maximumRenderers);

		_pushConstantRange = VkCreate::PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(ConstantBuffer), 0);
		_renderPass = renderPass;

		// Try to create a Pipeline cache, because we will create/destroy a lot of pipelines
		// based on the material properties
		{
			auto cacheCreateInfo = VkCreate::PipelineCacheCreateInfo();
			if (vkCreatePipelineCache(ctx->device, &cacheCreateInfo, nullptr, &_pipelineCache) != VK_SUCCESS)
			{
				_hasPipelineCache = true;
			}
		}

		// Create the descriptor pool
		VkDescriptorPoolSize poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, maximumRenderers }
		};

		auto descriptorPoolCreateInfo = VkCreate::DescriptorPoolCreateInfo(1, poolSizes, maximumRenderers);
		if (vkCreateDescriptorPool(_context->device, &descriptorPoolCreateInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Couldn't create mesh renderer descriptor pool !");
		}
	}
}