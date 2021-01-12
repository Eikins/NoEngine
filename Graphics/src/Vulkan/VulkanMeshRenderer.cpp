#include "Graphics/Vulkan/VulkanMeshRenderer.h"

#include <array>

#include "Graphics/Vulkan/VulkanUtils.hpp"

namespace Graphics
{
	void VulkanMeshRenderer::SetupCameraProperties(Core::Camera* camera)
	{
		_constantBuffer.viewMatrix = camera->GetTransform()->GetLocalToWorldMatrix().InverseTR();
		_constantBuffer.projectionMatrix = camera->GetProjectionMatrix();
		_constantBuffer.cameraWorldPosition = camera->GetTransform()->GetWorldPosition();
	}

	void VulkanMeshRenderer::SetDirectionalLight(const Math::Vector3& direction, const Math::Vector3& color)
	{
		_constantBuffer.directionalLightDirection = Math::Vector3::Normalize(direction);
		_constantBuffer.directionalLightColor = color;
	}

	VulkanMeshRenderer::BakedMaterial* VulkanMeshRenderer::BakeMaterial(Core::Material* material)
	{
		BakedMaterial& bakedMaterial = _bakedMaterials.insert(std::make_pair(material, BakedMaterial())).first->second;
		bakedMaterial.material = material;

		// Create Layouts
		{
			// Per Material Set Layout
			{
				// Material descriptors
				auto& properties = material->GetPropertyBlock().Properties();
				if (properties.size() > 0)
				{
					std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
					layoutBindings.push_back(VkCreate::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, 0));
					auto descriptorSetLayoutCreateInfo = VkCreate::DescriptorSetLayoutCreateInfo(layoutBindings.data(), layoutBindings.size());

					if (vkCreateDescriptorSetLayout(_context->device, &descriptorSetLayoutCreateInfo, nullptr, &bakedMaterial.perMaterialSetLayout) != VK_SUCCESS) {
						throw std::runtime_error("Failed to create per material descriptor set layout!");
					}

					// Create Descriptor set
					auto descriptorSetAllocateInfo = VkCreate::DescriptorSetAllocateInfo(_descriptorPool, &bakedMaterial.perMaterialSetLayout, 1);
					if (vkAllocateDescriptorSets(_context->device, &descriptorSetAllocateInfo, &bakedMaterial.descriptorSet) != VK_SUCCESS)
					{
						throw std::runtime_error("Couldn't create material descriptor set !");
					}

					// Create the uniform (Per Material) buffer
					// We don't use a device local buffer here because we will update these values frequently
					bakedMaterial.perMaterialBuffer = _context->CreateBuffer(
						sizeof(Math::Vector4) * properties.size(),
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
					);
					bakedMaterial.perMaterialBuffer.Bind();
					bakedMaterial.perMaterialBuffer.Map();

					// Bind buffer to the descriptor set
					auto bufferInfo = VkCreate::DescriptorBufferInfo(bakedMaterial.perMaterialBuffer.buffer, 0, sizeof(Math::Vector4) * properties.size());
					auto descriptorWrite = VkCreate::WriteDescriptorSet(bakedMaterial.descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, nullptr);
					descriptorWrite.pBufferInfo = &bufferInfo;
					vkUpdateDescriptorSets(_context->device, 1, &descriptorWrite, 0, nullptr);
				}
			}

			// Per Object Descriptor Set Layout
			{
				auto layoutBinding = VkCreate::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
				auto descriptorSetLayoutCreateInfo = VkCreate::DescriptorSetLayoutCreateInfo(&layoutBinding, 1);


				if (vkCreateDescriptorSetLayout(_context->device, &descriptorSetLayoutCreateInfo, nullptr, &bakedMaterial.perObjectSetLayout) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create per object descriptor set layout!");
				}
			}

			std::vector<VkDescriptorSetLayout> layouts;
			if (bakedMaterial.descriptorSet != VK_NULL_HANDLE)
			{
				layouts = { bakedMaterial.perObjectSetLayout, bakedMaterial.perMaterialSetLayout };
			}
			else
			{
				layouts = { bakedMaterial.perObjectSetLayout };
			}
			auto pipelineLayoutCreateInfo = VkCreate::PipelineLayoutCreateInfo(layouts.data(), layouts.size());
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

			if (vkCreateGraphicsPipelines(_context->device, _pipelineCache, 1, &pipelineCreateInfo, nullptr, &bakedMaterial.pipeline))
			{
				throw std::runtime_error("Couldn't create material pipeline !");
			}

			vkDestroyShaderModule(_context->device, vertexShaderModule, nullptr);
			vkDestroyShaderModule(_context->device, fragmentShaderModule, nullptr);
		}

		return &bakedMaterial;
	}

	VulkanMeshRenderer::BakedMesh* VulkanMeshRenderer::BakeMesh(Core::Mesh* mesh)
	{
		BakedMesh& bakedMesh = _bakedMeshes.insert(std::make_pair(mesh, BakedMesh())).first->second;
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

	void VulkanMeshRenderer::CheckMeshAndMaterial(BakedRenderer& bakedRenderer)
	{
		auto bakedMaterialIt = _bakedMaterials.find(bakedRenderer.renderer->material);
		if (bakedMaterialIt == _bakedMaterials.end())
		{
			bakedRenderer.bakedMaterial = BakeMaterial(bakedRenderer.renderer->material);
		}
		else
		{
			bakedRenderer.bakedMaterial = &bakedMaterialIt->second;
		}

		auto bakedMeshIt = _bakedMeshes.find(bakedRenderer.renderer->mesh);
		if (bakedMeshIt == _bakedMeshes.end())
		{
			bakedRenderer.bakedMesh = BakeMesh(bakedRenderer.renderer->mesh);
		}
		else
		{
			bakedRenderer.bakedMesh = &bakedMeshIt->second;
		}
	}

	VulkanMeshRenderer::BakedRenderer* VulkanMeshRenderer::BakeRenderer(Core::Renderer* renderer)
	{
		BakedRenderer& bakedRenderer = _bakedRenderers.insert(std::make_pair(renderer, BakedRenderer())).first->second;
		bakedRenderer.renderer = renderer;

		CheckMeshAndMaterial(bakedRenderer);

		// Create Descriptor set
		auto descriptorSetAllocateInfo = VkCreate::DescriptorSetAllocateInfo(_descriptorPool, &bakedRenderer.bakedMaterial->perObjectSetLayout, 1);
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

	void VulkanMeshRenderer::PrepareRenderers(std::vector<Core::Renderer>& renderers)
	{
		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = &renderers[i];
			auto& bakedRendererIt = _bakedRenderers.find(renderer);
			if (bakedRendererIt == _bakedRenderers.end())
			{
				BakeRenderer(renderer);
				renderer->hasChanged = false;
			}
			else if (renderer->hasChanged)
			{
				CheckMeshAndMaterial(bakedRendererIt->second);
			}
		}
	}

	void VulkanMeshRenderer::UpdateBuffers(std::vector<Core::Renderer>& renderers)
	{
		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = &renderers[i];
			auto bakedRendererIt = _bakedRenderers.find(renderer);

			if (bakedRendererIt != _bakedRenderers.end())
			{
				auto& bakedRenderer = bakedRendererIt->second;
				MeshPerObjectBuffer perObjectBufferData
				{
					renderer->GetTransform()->GetLocalToWorldMatrix()
				};

				memcpy(bakedRenderer.perObjectBuffer.mappedMemory, &perObjectBufferData, sizeof(perObjectBufferData));
				bakedRenderer.perObjectBuffer.Flush();

				if (renderer->material->GetPropertyBlock().HasChanged())
				{
					auto& propertyBlock = renderer->material->GetPropertyBlock();
					auto bakedMaterialIt = _bakedMaterials.find(renderer->material);
					if (bakedMaterialIt != _bakedMaterials.end())
					{
						auto& bakedMaterial = bakedMaterialIt->second;
						for (size_t i = 0; i < propertyBlock.Properties().size(); i++)
						{
							auto ptr = static_cast<Math::Vector4*>(bakedMaterial.perMaterialBuffer.mappedMemory) + i;
							memcpy(ptr, &propertyBlock.Properties()[i].vectorValue, sizeof(Math::Vector4));
						}
						bakedMaterial.perMaterialBuffer.Flush();
					}
				}
			}
		}
	}

	void VulkanMeshRenderer::RecordDrawCommands(VkCommandBuffer commandBuffer, std::vector<Core::Renderer>& renderers, float width, float height)
	{
		VkViewport viewport = VkCreate::Viewport(0, 0, width, height);
		auto scissor = VkCreate::Rect2D(0, 0, width, height);

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkDeviceSize offsets[] = { 0 };

		for (int i = 0; i < renderers.size(); i++)
		{
			auto renderer = &renderers[i];
			if (renderer->IsEnabled() == false || renderer->GetGameObject()->IsActive() == false) continue;
			auto bakedRendererIt = _bakedRenderers.find(renderer);

			if (bakedRendererIt != _bakedRenderers.end())
			{
				auto& bakedRenderer = bakedRendererIt->second;
				auto bakedMaterial = bakedRenderer.bakedMaterial;
				auto bakedMesh = bakedRenderer.bakedMesh;
				
				std::vector<VkDescriptorSet> descriptorSets;
				if (bakedMaterial->descriptorSet != VK_NULL_HANDLE)
				{
					descriptorSets = { bakedRenderer.descriptorSet, bakedMaterial->descriptorSet };
				}
				else
				{
					descriptorSets = { bakedRenderer.descriptorSet };
				}
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bakedMaterial->pipeline);
				vkCmdPushConstants(commandBuffer, bakedMaterial->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ConstantBuffer), &_constantBuffer);
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bakedMaterial->pipelineLayout, 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, &bakedMesh->vertices.buffer, offsets);
				vkCmdBindIndexBuffer(commandBuffer, bakedMesh->indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdDrawIndexed(commandBuffer, bakedMesh->indexCount, 1, 0, 0, 0);
			}
		}
	}

	void VulkanMeshRenderer::Init(VulkanContext* ctx, VkRenderPass renderPass, uint32_t maximumMaterials, uint32_t maximumMeshes, uint32_t maximumRenderers)
	{
		_context = ctx;

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
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, maximumRenderers + maximumMaterials }
		};

		auto descriptorPoolCreateInfo = VkCreate::DescriptorPoolCreateInfo(1, poolSizes, maximumRenderers);
		if (vkCreateDescriptorPool(_context->device, &descriptorPoolCreateInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Couldn't create mesh renderer descriptor pool !");
		}
	}

	void VulkanMeshRenderer::Release()
	{
		for (auto& it : _bakedMeshes) {
			auto& bakedMesh = it.second;
			bakedMesh.vertices.Release();
			bakedMesh.indices.Release();
		}

		for (auto& it : _bakedMaterials) {
			auto& bakedMaterial = it.second;
			vkDestroyPipeline(_context->device, bakedMaterial.pipeline, nullptr);
			vkDestroyPipelineLayout(_context->device, bakedMaterial.pipelineLayout, nullptr);
			if (bakedMaterial.perMaterialSetLayout != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorSetLayout(_context->device, bakedMaterial.perMaterialSetLayout, nullptr);
			}
			vkDestroyDescriptorSetLayout(_context->device, bakedMaterial.perObjectSetLayout, nullptr);
			bakedMaterial.perMaterialBuffer.Release();
		}

		for (auto& it : _bakedRenderers) {
			auto& bakedRenderer = it.second;
			bakedRenderer.perObjectBuffer.Release();
		}

		vkDestroyDescriptorPool(_context->device, _descriptorPool, nullptr);
		vkDestroyPipelineCache(_context->device, _pipelineCache, nullptr);
	}
}