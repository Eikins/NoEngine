#pragma once

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_vulkan.h"

#include "Graphics/Vulkan/VulkanContext.h"

namespace Graphics
{
    class VulkanImGuiRenderer
    {
    private:
        GLFWwindow* _window;
        VulkanContext _ctx;
        ImGui_ImplVulkanH_Window _mainWindowData;
        VkDescriptorPool _descriptorPool;
        VkCommandPool _commandPool;
        bool _swapchainRebuild = false;

        void CreateDescriptorPool();
        void CreateCommandPool();

        void SetupVulkanWindow(int width, int height);
        void CleanupVulkanWindow();

        VkCommandBuffer ImBeginCommand();
        void ImEndCommand(VkCommandBuffer commandBuffer);

        void FrameRender(ImDrawData* drawData);
        void FramePresent();

        static void CheckVkResult(VkResult result);
    public:
        void Init(GLFWwindow* window, VulkanContext ctx);
        void Draw();
        void Release();
    };
}