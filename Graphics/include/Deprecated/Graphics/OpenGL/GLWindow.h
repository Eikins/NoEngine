#pragma once

#include <GLFW/glfw3.h>

#include "Graphics/Window.h"

namespace Graphics
{
	class GLRenderDevice;
	class GLWindow : public Window
	{
		friend GLRenderDevice;
	private:
		GLFWwindow* _handle;
	public:
		virtual void SetTitle(const std::string& title) override;

		virtual bool ShouldClose() override;
		virtual void PollEvents() override;

		virtual void Release() override;
		virtual void* GetNativeHandle() override;

		static void GLFWErrorCallback(int id, const char* description);
	};
}