#pragma once

#include <vulkan/vulkan.h>

#include "Graphics/Vulkan/VulkanContext.h"

#include "Core/Components/Camera.h"
#include "Core/Components/Renderer.h"

namespace Graphics
{
	class VulkanMeshRenderer
	{
    private:
        struct ConstantBuffer
        {
            Math::Matrix4x4 viewMatrix;
            Math::Matrix4x4 projectionMatrix;
            Math::Vector3 worldCameraPosition;
        } _constantBuffer;

        VulkanContext* _context = nullptr;

        VkPushConstantRange _pushConstantRange;
        VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        bool _hasPipelineCache = false;
	public:
        void Init(VulkanContext* ctx);

        void SetupCameraProperties(Core::Camera& camera);

        void PrepareRenderers(const std::vector<Core::Renderer*> renderers);
		void RecordDrawCommands(VkCommandBuffer commandBuffer);
	};
}