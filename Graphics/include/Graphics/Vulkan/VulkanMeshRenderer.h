#pragma once

#include <vulkan/vulkan.h>

#include <unordered_map>

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

        std::unordered_map<Core::Material*, BakedMaterial> _bakedMaterials;
        std::unordered_map<Core::Mesh*, BakedMesh> _bakedMeshes;
        std::unordered_map<Core::Renderer*, BakedRenderer> _bakedRenderers;

        // TODO : optimise using a cache in Core
        BakedMaterial* BakeMaterial(Core::Material* material);
        BakedMesh* BakeMesh(Core::Mesh* mesh);
        BakedRenderer* BakeRenderer(Core::Renderer* renderer);
	public:
        void Init(VulkanContext* ctx, VkRenderPass renderPass, uint32_t materialCapacity = 8, uint32_t meshCapacity = 64, uint32_t rendererCapacity = 512);

        void SetupCameraProperties(Core::Camera& camera);

        void PrepareRenderers(std::vector<Core::Renderer>& renderers);
        void UpdateBuffers(std::vector<Core::Renderer>& renderers);
		void RecordDrawCommands(VkCommandBuffer commandBuffer, std::vector<Core::Renderer>& renderers, float width, float height);

        void Release();
	};
}