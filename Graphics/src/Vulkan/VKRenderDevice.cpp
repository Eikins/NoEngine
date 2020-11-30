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

#include "Graphics/Vulkan/VKPipelineState.h"
#include "Graphics/Vulkan/VKRenderDevice.h"
#include "Graphics/Vulkan/VKShader.h"
#include "Graphics/Vulkan/VKWindow.h"

namespace Graphics
{
	std::vector<char> VKReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (file.is_open() == false)
		{
			throw std::runtime_error("Failed to open file");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char>buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

#pragma region Debug Callbacks
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData)
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
		const VkAllocationCallbacks *pAllocator,
		VkDebugUtilsMessengerEXT *pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
#pragma endregion

#pragma region Internal Methods
	void VKRenderDevice::CreateInstance()
	{
		bool validationLayerSupported = CheckValidationLayerSupport();

		if (kEnableValidationLayers && validationLayerSupported == false) {
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "NoEngine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		// No extensions yet
		appInfo.pNext = nullptr;

		// Get extensions
		auto extensions = GetRequiredExtensions();

		// Create the instance descriptor
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Validation layers
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (kEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
			createInfo.ppEnabledLayerNames = kValidationLayers.data();

			debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugCallback;
			debugCreateInfo.pUserData = nullptr;
			debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create a Vulkan instance!");
		}
	}

	void VKRenderDevice::SetupDebugMessenger()
	{
		if (kEnableValidationLayers == false)
		{
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;
		debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;


		if (CreateDebugUtilsMessengerEXT(_instance, &debugCreateInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to set up debug messenger!");
		}
	}

	bool VKRenderDevice::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : kValidationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VKRenderDevice::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (kEnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VKRenderDevice::CreateSurface()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(_window->GetNativeHandle());
		VkSurfaceKHR* surface = &(static_cast<VKWindow*>(_window)->_surface);
		if (glfwCreateWindowSurface(_instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a window surface!");
		}
	}

	void VKRenderDevice::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

		if (deviceCount == 0)
		{
			throw std::runtime_error("Failed to find GPU(s) with Vulkan support!");
		}
		else
		{
			for (const auto &device : devices)
			{
				if (IsDeviceSuitable(device))
				{
					_physicalDevice = device;
					break;
				}
			}

			if (_physicalDevice == VK_NULL_HANDLE)
			{
				throw std::runtime_error("Failed to find a suitable GPU!");
			}
		}
	}

	bool VKRenderDevice::IsDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// ====================================
		// Add device feature requirements here
		// ====================================

		QueueFamilyIndices indices = FindQueueFamilies(device);
		bool extensionsSupported = CheckDeviceExtensionsSupport(device);
		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = swapChainSupport.formats.empty() == false && swapChainSupport.presentModes.empty() == false;
		}

		return indices.IsComplete() && extensionsSupported && swapChainAdequate;
	}

	bool VKRenderDevice::CheckDeviceExtensionsSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(kDeviceExtensions.begin(), kDeviceExtensions.end());

		for (const auto &extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VKRenderDevice::FindQueueFamilies(VkPhysicalDevice device)
	{
		VkSurfaceKHR surface = static_cast<VKWindow*>(_window)->_surface;
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			VkQueueFamilyProperties &queueFamily = queueFamilies[i];
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			// Does this Queue Family is a Drawing family
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}
		}

		return indices;
	}

	SwapChainSupportDetails VKRenderDevice::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		VkSurfaceKHR surface = static_cast<VKWindow*>(_window)->_surface;
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount = 0;
		uint32_t presentModeCount = 0;

		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void VKRenderDevice::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {
			indices.graphicsFamily.value(),
			indices.presentFamily.value()
		};

		// We will create 1 queue here, if we want to have more, change to a C array
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		// No feature required yet
		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(kDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = kDeviceExtensions.data();

		// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#extendingvulkan-layers-devicelayerdeprecation
		// Vulkan no longer distinct between device & instance validation layers
		// Keep checking for compatibility with old Vulkan Devices
		if (kEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
			createInfo.ppEnabledLayerNames = kValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a logical device!");
		}

		vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
		vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);

	}

	VkSurfaceFormatKHR VKRenderDevice::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		// Find sRGB format here
		for (const auto &availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VKRenderDevice::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto &availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VKRenderDevice::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR & capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			uint32_t width = _window->GetDescriptor().width;
			uint32_t height = _window->GetDescriptor().height;
			VkExtent2D actualExtent = { width, height };
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	}
#pragma endregion

	RenderDevice* VKRenderDevice::CreateDevice(const WindowDescriptor& descriptor)
	{
		// First of all, init GLFW
		glfwInit();

		// Our Engine don't support multiple windows atm
		glfwSetErrorCallback(&VKWindow::GLFWErrorCallback);

		// Using Vulkan, do not create OpenGL Context here
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		auto window = new VKWindow();
		window->_handle = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title.c_str(), nullptr, nullptr);

		auto renderDevice = new VKRenderDevice();
		renderDevice->_window = window;
		renderDevice->CreateInstance();
		renderDevice->SetupDebugMessenger();
		renderDevice->CreateSurface();
		renderDevice->PickPhysicalDevice();
		renderDevice->CreateLogicalDevice();
		
		return renderDevice;
	}

#pragma region RenderDevice Methods
	Window* VKRenderDevice::GetWindow()
	{
		return _window;
	}

	Buffer* VKRenderDevice::CreateBuffer(const BufferDescriptor &descriptor, const BufferData &data)
	{
		return nullptr;
	}

	Shader* VKRenderDevice::CreateShader(const ShaderCreationDescriptor &descriptor)
	{
		auto shaderCode = VKShader::ReadFile(descriptor.filename);
		auto shader = new VKShader();
		shader->_device = this;
		shader->_descriptor = *(descriptor.pDesc);
		
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(_device, &createInfo, nullptr, &shader->_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}

		return shader;
	}

	SwapChain* VKRenderDevice::CreateSwapChain()
	{
		auto swapChain = new VKSwapChain();
		swapChain->_device = this;
		
		VkSurfaceKHR surface = static_cast<VKWindow*>(_window)->_surface;
		SwapChainSupportDetails swapchainSupportDetails = QuerySwapChainSupport(_physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapChainSurfaceFormat(swapchainSupportDetails.formats);
		VkPresentModeKHR presentMode = ChooseSwapChainPresentMode(swapchainSupportDetails.presentModes);
		VkExtent2D extent = ChooseSwapChainExtent(swapchainSupportDetails.capabilities);

		// Sticking to the minimum may makes us wait on the driver to complete internal operations before we can aquire a new frame.
		uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
		uint32_t maxImageCount = swapchainSupportDetails.capabilities.maxImageCount;
		if (maxImageCount > 0 && imageCount > maxImageCount)
		{
			imageCount = maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			// If both queue are separate
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapchainSupportDetails.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &(swapChain->_handle)) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Swapchain!");
		}

		vkGetSwapchainImagesKHR(_device, swapChain->_handle, &imageCount, nullptr);
		swapChain->_images.resize(imageCount);
		vkGetSwapchainImagesKHR(_device, swapChain->_handle, &imageCount, swapChain->_images.data());

		swapChain->_imageFormat = surfaceFormat.format;
		swapChain->_extent = extent;

		swapChain->CreateImageViews();

		return swapChain;
	}

	PipelineState* VKRenderDevice::CreatePipelineState(const PipelineStateCreationDescriptor &descriptor)
	{
		auto pipelineState = new VKPipelineState();
		pipelineState->_device = this;

		VkPipelineShaderStageCreateInfo vertexShaderStage{};
		vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStage.module = static_cast<VKShader*>(descriptor.pVertexShader)->_handle;
		vertexShaderStage.pName = descriptor.vertexEntrypoint.c_str();

		VkPipelineShaderStageCreateInfo fragmentShaderStage{};
		fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStage.module = static_cast<VKShader*>(descriptor.pPixelShader)->_handle;
		fragmentShaderStage.pName = descriptor.pixelEntrypoint.c_str();

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStage, fragmentShaderStage };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = descriptor.viewport.x;
		viewport.y = descriptor.viewport.y;
		viewport.width = descriptor.viewport.width;
		viewport.height = descriptor.viewport.height;
		viewport.minDepth = descriptor.viewport.minDepth;
		viewport.maxDepth = descriptor.viewport.maxDepth;

		VkExtent2D scissorExtent{};
		scissorExtent.width = static_cast<uint32_t>(descriptor.viewport.width);
		scissorExtent.height = static_cast<uint32_t>(descriptor.viewport.height);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = scissorExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = descriptor.rasterizerState.clampDepth ? VK_TRUE :  VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.lineWidth = 1.0f;

		switch (descriptor.rasterizerState.polygonMode)
		{
		case PolygonMode::FILL:
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			break;
		case PolygonMode::LINE:
			rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
			break;
		case PolygonMode::POINT:
			rasterizer.polygonMode = VK_POLYGON_MODE_POINT;
			break;
		default:
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			break;
		}

		switch (descriptor.rasterizerState.cullMode)
		{
		case CullMode::BACK:
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			break;
		case CullMode::FRONT:
			rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
			break;
		case CullMode::FRONT_AND_BACK:
			rasterizer.cullMode = VK_CULL_MODE_FRONT_AND_BACK;
			break;
		default:
			rasterizer.cullMode = VK_CULL_MODE_NONE;
			break;
		}

		rasterizer.frontFace = descriptor.rasterizerState.windingOrder == WindingOrder::CLOCKWISE ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional


		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = descriptor.blendState.enabled ? VK_TRUE : VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &pipelineState->_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

		// TODO : Create a RenderPass object
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = static_cast<VKSwapChain*>(descriptor.pSwapChain)->_imageFormat;
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

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &pipelineState->_renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = pipelineState->_layout;
		pipelineInfo.renderPass = pipelineState->_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &(pipelineState->_pipeline)) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		return pipelineState;
	}

	CommandBuffer* VKRenderDevice::CreateCommandBuffer()
	{
		return nullptr;
	}

	VkDevice& VKRenderDevice::GetVulkanDevice()
	{
		return _device;
	}
#pragma endregion

#pragma region DeviceObject Methods

	void VKRenderDevice::Release()
	{
		if (kEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
		}

		vkDestroyDevice(_device, nullptr);

		VkSurfaceKHR surface = static_cast<VKWindow*>(_window)->_surface;
		vkDestroySurfaceKHR(_instance, surface, nullptr);

		vkDestroyInstance(_instance, nullptr);

		_window->Release();
		delete _window;

		glfwTerminate();
	}

	void* VKRenderDevice::GetNativeHandle()
	{
		return _instance;
	}
#pragma endregion

}