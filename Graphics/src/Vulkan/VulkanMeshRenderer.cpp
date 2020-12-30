#include "Graphics/Vulkan/VulkanMeshRenderer.h"

#include "Graphics/Vulkan/VulkanUtils.hpp"

namespace Graphics
{
	void VulkanMeshRenderer::SetupCameraProperties(Core::Camera& camera)
	{
		_constantBuffer.viewMatrix = camera.GetTransform()->GetLocalToWorldMatrix().InverseTR();
		_constantBuffer.projectionMatrix = camera.GetProjectionMatrix();
	}

	void VulkanMeshRenderer::PrepareRenderers(const std::vector<Core::Renderer*> renderers)
	{

	}

	void VulkanMeshRenderer::Init(VulkanContext* ctx)
	{
		_pushConstantRange = VkCreate::PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(ConstantBuffer), 0);

		// Try to create a Pipeline cache, because we will create/destroy a lot of pipelines
		// based on the material properties
		{
			auto cacheCreateInfo = VkCreate::PipelineCacheCreateInfo();
			if (vkCreatePipelineCache(ctx->device, &cacheCreateInfo, nullptr, &_pipelineCache) != VK_SUCCESS)
			{
				_hasPipelineCache = true;
			}
		}


	}
}