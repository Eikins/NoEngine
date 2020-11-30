#pragma once

#include <optional>

#include "vulkan/vulkan.h"

#include "Graphics/RenderDevice.h"
#include "Graphics/Vulkan/VKSwapChain.h"

namespace Graphics
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	class VKRenderDevice : public RenderDevice
	{
	private:
		const std::vector<const char*> kValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		const std::vector<const char *> kDeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifndef NDEBUG
		const bool kEnableValidationLayers = false;
#else
		const bool kEnableValidationLayers = true;
#endif // NDEBUG

		VkInstance _instance = VK_NULL_HANDLE;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _device = VK_NULL_HANDLE;

		VkQueue _graphicsQueue = VK_NULL_HANDLE;
		VkQueue _presentQueue = VK_NULL_HANDLE;

		VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;

		void CreateInstance();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();

		void SetupDebugMessenger();

		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionsSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		// TODO : need to be rethink as SwapChain creation descriptor
		VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR & capabilities);


	public:
		virtual Window* GetWindow() override;
		virtual Buffer* CreateBuffer(const BufferDescriptor& descriptor, const BufferData &data) override;
		virtual Shader* CreateShader(const ShaderCreationDescriptor& descriptor) override;
		virtual SwapChain* CreateSwapChain() override;
		virtual PipelineState* CreatePipelineState(const PipelineStateCreationDescriptor& descriptor) override;
		virtual CommandBuffer* CreateCommandBuffer() override;

		virtual void Release() override;
		virtual void* GetNativeHandle() override;

		VkDevice& GetVulkanDevice();
		
		static RenderDevice* CreateDevice(const WindowDescriptor& descriptor);
	};
}