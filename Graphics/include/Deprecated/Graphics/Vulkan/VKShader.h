#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/Shader.h"

namespace Graphics
{
	class VKRenderDevice;
	class VKShader : public Shader
	{
		friend VKRenderDevice;
	private:
		VkShaderModule _handle;
	public:
		virtual void Release() override;
		virtual void* GetNativeHandle() const override;

		static std::vector<char> ReadFile(const std::string& filename);
	};
}