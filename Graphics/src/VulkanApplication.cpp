#include "Graphics/VulkanApplication.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

static void glfwError(int id, const char* description)
{
	std::cerr << description << std::endl;
}

namespace Graphics {
	class VulkanApplicationImpl {
	public:
		void Run();
	private:
		const std::vector<const char*> kValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifndef NDEBUG
		const bool kEnableValidationLayers = false;
#else
		const bool kEnableValidationLayers = true;
#endif // NDEBUG

		GLFWwindow *_window;
		VkInstance _instance;

		void InitWindow();
		void InitVulkan();
		void MainLoop();
		void Cleanup();

		void CreateInstance();
		bool CheckValidationLayerSupport();
	};
}


void Graphics::VulkanApplicationImplDeleter::operator()(VulkanApplicationImpl* impl) {
	delete impl;
}

Graphics::VulkanApplication::VulkanApplication() : _impl(new VulkanApplicationImpl()) {};

void Graphics::VulkanApplication::Run()
{
	_impl->Run();
}

void Graphics::VulkanApplicationImpl::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void Graphics::VulkanApplicationImpl::InitWindow()
{
	glfwSetErrorCallback(&glfwError);
	glfwInit();

	// Using Vulkan, do not create OpenGL Context here
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(800, 600, "NoEngine", nullptr, nullptr);
}

void Graphics::VulkanApplicationImpl::InitVulkan()
{
	CreateInstance();
}

void Graphics::VulkanApplicationImpl::MainLoop()
{
	while (glfwWindowShouldClose(_window) == false)
	{
		glfwPollEvents();
	}
}

void Graphics::VulkanApplicationImpl::Cleanup()
{
	vkDestroyInstance(_instance, nullptr);

	glfwDestroyWindow(_window);
	glfwTerminate();
}

#pragma region Vulkan Initialisation
void Graphics::VulkanApplicationImpl::CreateInstance()
{
	if (kEnableValidationLayers && CheckValidationLayerSupport() == false) {
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

	// Get GLFW extensions
	uint32_t glfwExtensionCount;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Create the instance descriptor
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	// Validation layers
	if (kEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
		createInfo.ppEnabledLayerNames = kValidationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create a Vulkan instance!");
	}
}

bool Graphics::VulkanApplicationImpl::CheckValidationLayerSupport()
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
#pragma endregion
