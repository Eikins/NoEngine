#pragma once

#include <array>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanContext.h"

namespace Graphics
{
    class VulkanRenderer
    {
    public:
        GLFWwindow *_window = nullptr;

        void Init(const VulkanContext& ctx);
        void Cleanup();

        void DrawFrame();
    private:
        VulkanContext _context;
        VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> _swapChainImages;
        std::vector<VkImageView> _swapChainImageViews;
        std::vector<VkFramebuffer> _swapChainFramebuffers;
        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;

        VkRenderPass _renderPass;
        VkDescriptorSetLayout _descriptorSetLayout;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _pipeline;

        VkCommandPool _commandPool;
        // We have 1 command buffer per framebuffer
        std::vector<VkCommandBuffer> _commandBuffers;

        VkDescriptorPool _descriptorPool;
        std::vector<VkDescriptorSet> _descriptorSets;

        VkSemaphore _imageAvailableSemaphore;
        VkSemaphore _renderFinishedSemaphore;

        VkBuffer _vertexBuffer, _indexBuffer;
        VkDeviceMemory _vertexBufferMemory, _indexBufferMemory;
        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;

        bool _framebufferResized = false;

        void RecreateSwapChain();
        void CleanupSwapChain();

        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDescriptorSetLayout();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateUniformBuffers();
        void CreateDescriptorPool();
        void CreateDescriptorSets();
        void CreateCommandBuffers();
        void CreateSemaphores();

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize offset, VkDeviceSize size);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void UpdateUniformBuffer(int index);

        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

        friend class GraphicsContext;
        friend class GraphicsContextImpl;
    };
}