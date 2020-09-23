#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Graphics
{
	#pragma region Forward Dependency Shield
	// Used to avoid forward dependencies with vulkan and glfw
	class VulkanApplicationImpl;

	struct VulkanApplicationImplDeleter {
		void operator()(VulkanApplicationImpl* impl);
	};
	#pragma endregion

	class VulkanApplication {
	public:
		VulkanApplication();

		void Run();
	private:
		std::unique_ptr<VulkanApplicationImpl, VulkanApplicationImplDeleter> _impl;
	};
}