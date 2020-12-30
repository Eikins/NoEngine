#pragma once

#include <vulkan/vulkan.h>

#include "Graphics/Vulkan/VulkanContext.h"

#include "Core/Components/Camera.h"
#include "Core/Components/Renderer.h"

namespace Graphics
{
    struct MeshPerObjectBuffer
    {
        Math::Matrix4x4 model;
    };

	class VulkanMeshRenderer
	{
    private:
        struct BakedMaterial
        {
            Core::Material* material;
            VkDescriptorSetLayout descriptorSetLayout;
            VkPipelineLayout pipelineLayout;
            VkPipeline pipeline;
        };

        struct BakedMesh
        {
            Core::Mesh* mesh;
            VulkanBuffer vertices;
            VulkanBuffer indices;
            uint32_t indexCount;
        };

        struct BakedRenderer
        {
            Core::Renderer* renderer;
            BakedMaterial* bakedMaterial;
            BakedMesh* bakedMesh;
            VkDescriptorSet descriptorSet;
            VulkanBuffer perObjectBuffer;
        };

        struct ConstantBuffer
        {
            Math::Matrix4x4 viewMatrix;
            Math::Matrix4x4 projectionMatrix;
            Math::Vector3 worldCameraPosition;
        } _constantBuffer;

        VulkanContext* _context = nullptr;

        VkPushConstantRange _pushConstantRange;
        VkRenderPass _renderPass = VK_NULL_HANDLE;
        VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
        VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        bool _hasPipelineCache = false;

        uint32_t _materialLimit = 0;
        uint32_t _meshLimit = 0;
        uint32_t _rendererLimit = 0;
        std::vector<BakedMaterial> _bakedMaterials;
        std::vector<BakedMesh> _bakedMeshes;
        std::vector<BakedRenderer> _bakedRenderers;

        // TODO : optimise using a cache in Core
        BakedMaterial* FindBakedMaterial(Core::Material* material);
        BakedMaterial* BakeMaterial(Core::Material* material);

        BakedMesh* FindBakedMesh(Core::Mesh* mesh);
        BakedMesh* BakeMesh(Core::Mesh* mesh);

        BakedRenderer* FindBakedRenderer(Core::Renderer* renderer);
        BakedRenderer* BakeRenderer(Core::Renderer* renderer);
	public:
        void Init(VulkanContext* ctx, VkRenderPass renderPass, uint32_t maximumMaterials = 8, uint32_t maximumMeshes = 64, uint32_t maximumRenderers = 512);

        void SetupCameraProperties(Core::Camera& camera);

        void PrepareRenderers(const std::vector<Core::Renderer*>& renderers);
        void UpdateBuffers(const std::vector<Core::Renderer*>& renderers);
		void RecordDrawCommands(VkCommandBuffer commandBuffer, const std::vector<Core::Renderer*>& renderers, float width, float height);
	};
}