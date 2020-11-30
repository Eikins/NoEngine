#pragma once

#include <vulkan/vulkan.h>

#include "Graphics/Window.h"

namespace Graphics
{
	class VKRenderDevice;
	class VKWindow : public Window
	{
		friend VKRenderDevice;
	private:
		GLFWwindow* _handle;
		VkSurfaceKHR _surface;
	public:
		virtual void SetTitle(const std::string& title) override;

		virtual bool ShouldClose() override;
		virtual void PollEvents() override;

		virtual void Release() override;
		virtual void* GetNativeHandle() const override;

		static void GLFWErrorCallback(int id, const char* description);
	};
}