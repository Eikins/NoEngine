#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanWindow.h"
#include "Graphics/Vulkan/VulkanMeshRenderer.h"

#include "Graphics/Vulkan/VulkanImGui.h"

#include "Graphics/imgui/imgui_impl_glfw.h"

#include "Graphics/Vulkan/VulkanUtils.hpp"

namespace Graphics
{
#pragma region Graphics Context Implementation Prototype
    class GraphicsContextImpl
    {
    private:
        GLFWwindow* _window;
        Core::InputMaster* _inputMaster;
        VulkanContext _vkContext;

        VulkanSwapchain _swapchain;

        VulkanMeshRenderer _meshRenderer;
#ifdef NoEngine_Editor
        VulkanImGui _vkImGui;
#endif

        // Global Render Pass resources
        VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;;
        VkRenderPass _renderPass = VK_NULL_HANDLE;
        // We create one framebuffer per swapchain image
        std::vector<VkFramebuffer> _framebuffers;
        // We need one depth buffer that we will reuse internally
        struct {
            VkFormat format;
            VkImage image;
            VkDeviceMemory memory;
            VkImageView view;
        } _depth;
        
        VkSemaphore _imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore _renderFinishedSemaphore = VK_NULL_HANDLE;
        uint32_t _imageIndex;

        bool _framebufferResized = false;

        void PrepareRenderers(std::vector<Core::Renderer>& renderers)
        {
            _meshRenderer.PrepareRenderers(renderers);
        }

        void SetupCameraProperties(Core::Camera* camera)
        {
            camera->SetAspectRatio(static_cast<float>(_swapchain.extent.width) / _swapchain.extent.height);
            _meshRenderer.SetupCameraProperties(camera);
        }

        void SetDirectionalLight(const Math::Vector3& direction, const Math::Vector3& color)
        {
            _meshRenderer.SetDirectionalLight(direction, color);
        }

        bool BeginFrame()
        {
            VkResult result = _swapchain.AcquireImage(_imageAvailableSemaphore, &_imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                int width, height;
                glfwGetFramebufferSize(_window, &width, &height);
                OnWindowResize(width, height);
                return false;
            }
            else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            {
                throw std::runtime_error("failed to acquire swap chain image!");
            }

            _commandBuffer = _vkContext.CreateCommandBuffer(true);

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.04f, 0.05f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = _renderPass;
            renderPassInfo.framebuffer = _framebuffers[_imageIndex];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = _swapchain.extent;
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            return true;
        }

        void EndFrame()
        {
            vkCmdEndRenderPass(_commandBuffer);
        }

        void DrawRenderers(std::vector<Core::Renderer>& renderers)
        {
            _meshRenderer.UpdateBuffers(renderers);
            auto extent = _swapchain.extent;
            _meshRenderer.RecordDrawCommands(_commandBuffer, renderers, extent.width, extent.height);
        }

        void BeginEditorFrame()
        {
#ifdef NoEngine_Editor
            ImGui_ImplGlfw_NewFrame();
            _vkImGui.NewFrame();
#endif
        }

        void EndEditorFrame()
        {
#ifdef NoEngine_Editor
            _vkImGui.UpdateBuffers();
            _vkImGui.RecordDrawCommands(_commandBuffer);
#endif
        }

        void RenderAsync()
        {
            if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }

            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &_imageAvailableSemaphore;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &_commandBuffer;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &_renderFinishedSemaphore;

            if (vkQueueSubmit(_vkContext.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
                throw std::runtime_error("Failed to submit draw command buffer!");
            }

            VkResult result = _swapchain.PresentToScreen(_imageIndex, _renderFinishedSemaphore);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
                int width, height;
                glfwGetFramebufferSize(_window, &width, &height);
                OnWindowResize(width, height);
            }
            else if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
            }
        }

        void WaitForRenderCompletion()
        {
            vkDeviceWaitIdle(_vkContext.device);
            if (_commandBuffer != VK_NULL_HANDLE)
            {
                _vkContext.FreeCommandBuffer(_commandBuffer);
                _commandBuffer = VK_NULL_HANDLE;
            }
        }

        void Release()
        {
            vkDeviceWaitIdle(_vkContext.device);

            vkDestroyImageView(_vkContext.device, _depth.view, nullptr);
            vkDestroyImage(_vkContext.device, _depth.image, nullptr);
            vkFreeMemory(_vkContext.device, _depth.memory, nullptr);

            for (uint32_t i = 0; i < _framebuffers.size(); i++)
            {
                vkDestroyFramebuffer(_vkContext.device, _framebuffers[i], nullptr);
            }

            vkDestroySemaphore(_vkContext.device, _imageAvailableSemaphore, nullptr);
            vkDestroySemaphore(_vkContext.device, _renderFinishedSemaphore, nullptr);
            _vkImGui.Release();
            _meshRenderer.Release();
            vkDestroyRenderPass(_vkContext.device, _renderPass, nullptr);
            _swapchain.Release();
            _vkContext.Release();

            glfwDestroyWindow(_window);
            glfwTerminate();
        }

        void CreateResources()
        {
            _depth.format = _vkContext.FindSupportedFormat(
                { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );

            // Create Render Pass
            {
                // TODO : Create a RenderPass object
                VkAttachmentDescription colorAttachment{};
                colorAttachment.format = _swapchain.format;
                colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference colorAttachmentRef{};
                colorAttachmentRef.attachment = 0;
                colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentDescription depthAttachment{};
                depthAttachment.format = _depth.format;
                depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference depthAttachmentRef{};
                depthAttachmentRef.attachment = 1;
                depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass{};
                subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass.colorAttachmentCount = 1;
                subpass.pColorAttachments = &colorAttachmentRef;
                subpass.pDepthStencilAttachment = &depthAttachmentRef;

                VkSubpassDependency dependency{};
                dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
                dependency.dstSubpass = 0;
                dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                dependency.srcAccessMask = 0;
                dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

                VkRenderPassCreateInfo renderPassInfo{};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
                renderPassInfo.pAttachments = attachments.data();
                renderPassInfo.subpassCount = 1;
                renderPassInfo.pSubpasses = &subpass;
                renderPassInfo.dependencyCount = 1;
                renderPassInfo.pDependencies = &dependency;

                if (vkCreateRenderPass(_vkContext.device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create render pass!");
                }
            }

            // Create Frambuffers
            CreateDepthResources();
            CreateFramebuffers();

            // Create Semaphores
            {
                VkSemaphoreCreateInfo semaphoreInfo{};
                semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

                if (vkCreateSemaphore(_vkContext.device, &semaphoreInfo, nullptr, &_imageAvailableSemaphore) != VK_SUCCESS ||
                    vkCreateSemaphore(_vkContext.device, &semaphoreInfo, nullptr, &_renderFinishedSemaphore) != VK_SUCCESS) {

                    throw std::runtime_error("Failed to create semaphores!");
                }
            }
        }

        void CreateFramebuffers()
        {
            uint32_t imageCount = _swapchain.GetImageCount();
            _framebuffers.resize(imageCount);

            for (size_t i = 0; i < imageCount; i++) {
                std::array<VkImageView, 2> attachments = {
                    _swapchain.GetImageView(i),
                    _depth.view
                };

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = _renderPass;
                framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
                framebufferInfo.pAttachments = attachments.data();
                framebufferInfo.width = _swapchain.extent.width;
                framebufferInfo.height = _swapchain.extent.height;
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(_vkContext.device, &framebufferInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create framebuffer!");
                }
            }
        }

        void CreateDepthResources()
        {
            // Create Image
            {
                auto imageCreateInfo = VkCreate::Image2DCreateInfo(
                    _swapchain.extent.width, _swapchain.extent.height, _depth.format,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                );

                if (vkCreateImage(_vkContext.device, &imageCreateInfo, nullptr, &_depth.image) != VK_SUCCESS)
                {
                    throw std::runtime_error("Couldn't create depth image !");
                }
                VkMemoryRequirements memReqs{};
                vkGetImageMemoryRequirements(_vkContext.device, _depth.image, &memReqs);
                auto memAllocInfo = VkCreate::MemoryAllocateInfo();
                memAllocInfo.allocationSize = memReqs.size;
                memAllocInfo.memoryTypeIndex = _vkContext.FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

                if (vkAllocateMemory(_vkContext.device, &memAllocInfo, nullptr, &_depth.memory) != VK_SUCCESS) {
                    throw std::runtime_error("Couldn't create depth image memory !");
                }
                vkBindImageMemory(_vkContext.device, _depth.image, _depth.memory, 0);
            }

            // Create the image view
            {
                auto imageViewCreateInfo = VkCreate::Image2DViewCreateInfo(_depth.image, _depth.format, VK_IMAGE_ASPECT_DEPTH_BIT);
                if (vkCreateImageView(_vkContext.device, &imageViewCreateInfo, nullptr, &_depth.view) != VK_SUCCESS) {
                    throw std::runtime_error("Couldn't create depth image view !");
                }
            }

        }

        void OnWindowResize(uint32_t width, uint32_t height)
        {
            WaitForRenderCompletion();
            _swapchain.Recreate(width, height);

            vkDestroyImageView(_vkContext.device, _depth.view, nullptr);
            vkDestroyImage(_vkContext.device, _depth.image, nullptr);
            vkFreeMemory(_vkContext.device, _depth.memory, nullptr);
            for (uint32_t i = 0; i < _framebuffers.size(); i++)
            {
                vkDestroyFramebuffer(_vkContext.device, _framebuffers[i], nullptr);
            }

            CreateDepthResources();
            CreateFramebuffers();

            _framebufferResized = false;
        }

        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
        {
            auto ctx = reinterpret_cast<GraphicsContextImpl*>(glfwGetWindowUserPointer(window));
            ctx->_framebufferResized = true;
        }

        void CreateWindow(const WindowDescriptor& descriptor)
        {
            glfwSetErrorCallback(VulkanWindow::GLFWErrorCallback);
            glfwInit();

            // Using Vulkan, do not create OpenGL Context here
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, descriptor.resizable);

            _window = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title.c_str(), nullptr, nullptr);
            glfwSetWindowUserPointer(_window, this);
            if (descriptor.resizable)
            {
                glfwSetFramebufferSizeCallback(_window, FramebufferResizeCallback);
            }
        }

        friend GraphicsContext;
        friend GraphicsContext CreateGraphicsContext(const WindowDescriptor& windowDescriptor);
        friend void __GLFW__KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        friend void __GLFW__CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    };
#pragma endregion

#pragma region GLFW Input
    void __GLFW__KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto impl = static_cast<GraphicsContextImpl*>(glfwGetWindowUserPointer(window));

        Core::InputEventPhase phase =
            action == GLFW_PRESS ? Core::InputEventPhase::PRESS :
            action == GLFW_REPEAT ? Core::InputEventPhase::REPEAT :
            Core::InputEventPhase::RELEASED;

        impl->_inputMaster->DispatchInputEvent({ key, phase });
    }

    void __GLFW__CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto impl = static_cast<GraphicsContextImpl*>(glfwGetWindowUserPointer(window));
        auto mousePos = Math::Vector2(xpos, ypos);;
        impl->_inputMaster->mouseDelta = mousePos - impl->_inputMaster->mousePos;
        impl->_inputMaster->mousePos = mousePos;

    }
#pragma endregion


#pragma region Graphics Context
    GraphicsContext::GraphicsContext() {}

    Window& GraphicsContext::GetWindow()
    {
        return *_window;
    }

    void GraphicsContext::BindInputMaster(Core::InputMaster* inputMaster) { 
        _impl->_inputMaster = inputMaster;;
        glfwSetKeyCallback(static_cast<VulkanWindow*>(_window)->GetHandle(), __GLFW__KeyCallback);
        glfwSetCursorPosCallback(static_cast<VulkanWindow*>(_window)->GetHandle(), __GLFW__CursorPositionCallback);
    }

    void GraphicsContext::PrepareRenderers(std::vector<Core::Renderer>& renderers) { _impl->PrepareRenderers(renderers); }
    void GraphicsContext::SetupCameraProperties(Core::Camera* camera) { _impl->SetupCameraProperties(camera); }
    void GraphicsContext::SetDirectionalLight(const Math::Vector3& direction, const Math::Vector3& color) { _impl->SetDirectionalLight(direction, color); }

    bool GraphicsContext::BeginFrame() { return _impl->BeginFrame(); }
    void GraphicsContext::EndFrame() { _impl->EndFrame(); }
    void GraphicsContext::DrawRenderers(std::vector<Core::Renderer>& renderers) { _impl->DrawRenderers(renderers); }

    void GraphicsContext::BeginEditorFrame() { _impl->BeginEditorFrame(); }
    void GraphicsContext::EndEditorFrame() { _impl->EndEditorFrame(); }

    void GraphicsContext::RenderAsync() { _impl->RenderAsync(); }
    void GraphicsContext::WaitForRenderCompletion() { _impl->WaitForRenderCompletion(); }

    void GraphicsContext::DrawScene()
    {
        //_impl->_vkRenderer.DrawFrame();
    }

    void GraphicsContext::Release() { _impl->Release(); }

    GraphicsContext Graphics::CreateGraphicsContext(const WindowDescriptor& descriptor)
    {
        GraphicsContext context;
        context._impl = new GraphicsContextImpl();
        context._impl->CreateWindow(descriptor);

        auto vkWindow = new VulkanWindow(context._impl->_window);

        context._window = vkWindow;
        context._impl->_vkContext = VulkanContext::CreateVulkanContext(*vkWindow);
        auto vkContext = &context._impl->_vkContext;
        context._impl->_swapchain.Init(vkContext, descriptor.width, descriptor.height);
        context._impl->CreateResources();
        context._impl->_meshRenderer.Init(vkContext, context._impl->_renderPass);
        context._impl->_vkImGui.Init(vkContext, descriptor.width, descriptor.height);
        context._impl->_vkImGui.CreateResources(context._impl->_renderPass, vkContext->graphicsQueue);
        

        ImGui_ImplGlfw_InitForVulkan(vkWindow->GetHandle(), true);
        return context;
    }
#pragma endregion
}