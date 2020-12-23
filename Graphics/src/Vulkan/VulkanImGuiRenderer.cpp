#include "Graphics/Vulkan/VulkanImGuiRenderer.h"

#include <stdexcept>

namespace Graphics
{
    void VulkanImGuiRenderer::CheckVkResult(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("[vulkan][ImGuiRenderer] Error: VkResult " + std::to_string(result));
        }
    }

    void VulkanImGuiRenderer::CreateDescriptorPool()
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        if (vkCreateDescriptorPool(_ctx.device, &pool_info, nullptr, &_descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool!");
        }
    }

    void VulkanImGuiRenderer::CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = _ctx.queueFamilies;

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(_ctx.device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanImGuiRenderer::SetupVulkanWindow(int width, int height)
    {
        _mainWindowData.Surface = _ctx.surface;

        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        _mainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
            _ctx.physicalDevice,
            _mainWindowData.Surface, 
            requestSurfaceImageFormat, 
            (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
            requestSurfaceColorSpace
        );

        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        _mainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(_ctx.physicalDevice, _mainWindowData.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        ImGui_ImplVulkanH_CreateOrResizeWindow(_ctx.instance, _ctx.physicalDevice, _ctx.device, &_mainWindowData, _ctx.queueFamilies.graphicsFamily.value(), nullptr, width, height, 2);
    }

    void VulkanImGuiRenderer::CleanupVulkanWindow()
    {
        ImGui_ImplVulkanH_DestroyWindow(_ctx.instance, _ctx.device, &_mainWindowData, nullptr);
    }

    VkCommandBuffer VulkanImGuiRenderer::ImBeginCommand()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(_ctx.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanImGuiRenderer::ImEndCommand(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(_ctx.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(_ctx.graphicsQueue);

        vkFreeCommandBuffers(_ctx.device, _commandPool, 1, &commandBuffer);
    }

    void VulkanImGuiRenderer::Init(GLFWwindow* window, VulkanContext ctx)
    {
        _ctx = ctx;
        _window = window;

        CreateDescriptorPool();
        CreateCommandPool();
        ImGui_ImplVulkanH_Window* wd = &_mainWindowData;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        SetupVulkanWindow(width, height);

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = ctx.instance;
        init_info.PhysicalDevice = ctx.physicalDevice;
        init_info.Device = ctx.device;
        init_info.QueueFamily = ctx.queueFamilies.graphicsFamily.value();
        init_info.Queue = ctx.graphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = _descriptorPool;
        init_info.Allocator = nullptr;
        init_info.MinImageCount = 2;
        init_info.ImageCount = wd->ImageCount;
        init_info.CheckVkResultFn = CheckVkResult;
        ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

        auto commandBuffer = ImBeginCommand();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        ImEndCommand(commandBuffer);
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        _mainWindowData.ClearValue.color.float32[0] = 0.0f;
        _mainWindowData.ClearValue.color.float32[1] = 0.0f;
        _mainWindowData.ClearValue.color.float32[2] = 0.0f;
        _mainWindowData.ClearValue.color.float32[3] = 1.0f;
    }

    void VulkanImGuiRenderer::Draw()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        if (_swapchainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(_window, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(2);
                ImGui_ImplVulkanH_CreateOrResizeWindow(
                    _ctx.instance,
                    _ctx.physicalDevice,
                    _ctx.device,
                    &_mainWindowData,
                    _ctx.queueFamilies.graphicsFamily.value(), 
                    nullptr,
                    width,
                    height,
                    2
                );
                _mainWindowData.FrameIndex = 0;
                _swapchainRebuild = false;
            }
        }

        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            FrameRender(drawData);
            FramePresent();
            // For the moment, we will stick to a synchronised app, it's not the best for performance
            // but will be simpler to dev the engine.
            vkQueueWaitIdle(_ctx.presentQueue);
        }
    }

    void VulkanImGuiRenderer::FrameRender(ImDrawData* drawData)
    {
        VkResult err;

        VkSemaphore image_acquired_semaphore = _mainWindowData.FrameSemaphores[_mainWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = _mainWindowData.FrameSemaphores[_mainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
        err = vkAcquireNextImageKHR(_ctx.device, _mainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &_mainWindowData.FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            _swapchainRebuild = true;
            return;
        }
        else if (err != VK_SUBOPTIMAL_KHR)
        {
            CheckVkResult(err);
        }

        ImGui_ImplVulkanH_Frame* fd = &_mainWindowData.Frames[_mainWindowData.FrameIndex];
        {
            err = vkWaitForFences(_ctx.device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
            CheckVkResult(err);

            err = vkResetFences(_ctx.device, 1, &fd->Fence);
            CheckVkResult(err);
        }
        {
            err = vkResetCommandPool(_ctx.device, fd->CommandPool, 0);
            CheckVkResult(err);
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            CheckVkResult(err);
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = _mainWindowData.RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = _mainWindowData.Width;
            info.renderArea.extent.height = _mainWindowData.Height;
            info.clearValueCount = 1;
            info.pClearValues = &_mainWindowData.ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &image_acquired_semaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &render_complete_semaphore;

            err = vkEndCommandBuffer(fd->CommandBuffer);
            CheckVkResult(err);
            err = vkQueueSubmit(_ctx.graphicsQueue, 1, &info, fd->Fence);
            CheckVkResult(err);
        }
    }

    void VulkanImGuiRenderer::FramePresent()
    {
        if (_swapchainRebuild)
            return;

        VkSemaphore render_complete_semaphore = _mainWindowData.FrameSemaphores[_mainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &_mainWindowData.Swapchain;
        info.pImageIndices = &_mainWindowData.FrameIndex;
        VkResult err = vkQueuePresentKHR(_ctx.presentQueue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR)
        {
            _swapchainRebuild = true;
            return;
        }
        else if (err == VK_SUBOPTIMAL_KHR)
        {
            _swapchainRebuild = true;
        }
        else
        {
            CheckVkResult(err);
        }
        _mainWindowData.SemaphoreIndex = (_mainWindowData.SemaphoreIndex + 1) % _mainWindowData.ImageCount; // Now we can use the next set of semaphores
    }

    void VulkanImGuiRenderer::Release()
    {
        vkDeviceWaitIdle(_ctx.device);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        CleanupVulkanWindow();
        vkDestroyDescriptorPool(_ctx.device, _descriptorPool, nullptr);
        vkDestroyCommandPool(_ctx.device, _commandPool, nullptr);
    }
}