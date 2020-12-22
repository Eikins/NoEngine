#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/Vulkan/VulkanWindow.h"

namespace Graphics
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanContext
    {
    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface(const VulkanWindow& vulkanWindow);
        void PickPhysicalDevice();
        void CreateLogicalDevice();

        bool CheckDeviceExtensionsSupport(VkPhysicalDevice device);
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

        bool IsDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
    public:
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        VkInstance instance = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;

        QueueFamilyIndices queueFamilies;

        SwapchainSupportDetails QuerySwapchainSupport();
        void Release();

        // Constants
        static const std::vector<const char*> kValidationLayers;
        static const std::vector<const char *> kDeviceExtensions;

#ifndef NDEBUG
        static const bool kEnableValidationLayers = false;
#else
        static const bool kEnableValidationLayers = true;
#endif // NDEBUG

        static VulkanContext CreateVulkanContext(const VulkanWindow& vulkanWindow);
    };
}