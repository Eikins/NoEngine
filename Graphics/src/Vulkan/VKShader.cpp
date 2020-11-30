#include <fstream>

#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VKRenderDevice.h"
#include "Graphics/Vulkan/VKShader.h"

namespace Graphics
{

	void VKShader::Release()
	{
		VkDevice device = static_cast<VKRenderDevice*>(_device)->GetVulkanDevice();
		vkDestroyShaderModule(device, _handle, nullptr);
	}

	void* VKShader::GetNativeHandle() const
	{
		return &_handle;
	}

	std::vector<char> VKShader::ReadFile(const std::string& filename)
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

}