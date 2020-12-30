#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanWindow.h"
#include "Graphics/Vulkan/VulkanMeshRenderer.h"

#include "Graphics/Vulkan/VulkanImGui.h"

#include "Graphics/imgui/imgui_impl_glfw.h"

namespace Graphics
{
#pragma region Graphics Context Implementation Prototype
    class GraphicsContextImpl
    {
    private:
        GLFWwindow* _window;
        VulkanContext _vkContext;

        VulkanSwapchain _swapchain;

        VulkanMeshRenderer _meshRenderer;
#ifdef NoEngine_Editor
        VulkanImGui _vkImGui;
#endif

        VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;;
        VkRenderPass _renderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> _framebuffers;

        VkSemaphore _imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore _renderFinishedSemaphore = VK_NULL_HANDLE;
        uint32_t _imageIndex;

        bool _framebufferResized = false;

        void PrepareRenderers(const std::vector<Core::Renderer*>& renderers)
        {
            _meshRenderer.PrepareRenderers(renderers);
        }

        void SetupCameraProperties(Core::Camera& camera)
        {
            _meshRenderer.SetupCameraProperties(camera);
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

            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = _renderPass;
            renderPassInfo.framebuffer = _framebuffers[_imageIndex];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = _swapchain.extent;
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            return true;
        }

        void EndFrame()
        {
            vkCmdEndRenderPass(_commandBuffer);
        }

        void DrawRenderers(const std::vector<Core::Renderer*>& renderers)
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

            for (uint32_t i = 0; i < _framebuffers.size(); i++)
            {
                vkDestroyFramebuffer(_vkContext.device, _framebuffers[i], nullptr);
            }
            vkDestroySemaphore(_vkContext.device, _imageAvailableSemaphore, nullptr);
            vkDestroySemaphore(_vkContext.device, _renderFinishedSemaphore, nullptr);
            _vkImGui.Release();
            vkDestroyRenderPass(_vkContext.device, _renderPass, nullptr);
            _swapchain.Release();
            _vkContext.Release();

            glfwDestroyWindow(_window);
            glfwTerminate();
        }

        void CreateResources()
        {
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

                VkSubpassDescription subpass{};
                subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass.colorAttachmentCount = 1;
                subpass.pColorAttachments = &colorAttachmentRef;

                VkSubpassDependency dependency{};
                dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
                dependency.dstSubpass = 0;
                dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dependency.srcAccessMask = 0;
                dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                VkRenderPassCreateInfo renderPassInfo{};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                renderPassInfo.attachmentCount = 1;
                renderPassInfo.pAttachments = &colorAttachment;
                renderPassInfo.subpassCount = 1;
                renderPassInfo.pSubpasses = &subpass;
                renderPassInfo.dependencyCount = 1;
                renderPassInfo.pDependencies = &dependency;

                if (vkCreateRenderPass(_vkContext.device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create render pass!");
                }
            }

            // Create Frambuffers
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
                VkImageView attachments[] = {
                    _swapchain.GetImageView(i)
                };

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = _renderPass;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = attachments;
                framebufferInfo.width = _swapchain.extent.width;
                framebufferInfo.height = _swapchain.extent.height;
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(_vkContext.device, &framebufferInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create framebuffer!");
                }
            }
        }

        void OnWindowResize(uint32_t width, uint32_t height)
        {
            WaitForRenderCompletion();
            _swapchain.Recreate(width, height);
            for (uint32_t i = 0; i < _framebuffers.size(); i++)
            {
                vkDestroyFramebuffer(_vkContext.device, _framebuffers[i], nullptr);
            }
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
    };
#pragma endregion

#pragma region Graphics Context
    GraphicsContext::GraphicsContext() {}

    Window& GraphicsContext::GetWindow()
    {
        return *_window;
    }

    void GraphicsContext::PrepareRenderers(const std::vector<Core::Renderer*>& renderers) { _impl->PrepareRenderers(renderers); }
    void GraphicsContext::SetupCameraProperties(Core::Camera& camera) { _impl->SetupCameraProperties(camera); }

    bool GraphicsContext::BeginFrame() { return _impl->BeginFrame(); }
    void GraphicsContext::EndFrame() { _impl->EndFrame(); }
    void GraphicsContext::DrawRenderers(const std::vector<Core::Renderer*>& renderers) { _impl->DrawRenderers(renderers); }

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