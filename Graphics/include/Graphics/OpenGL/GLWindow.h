#pragma once

#include <GLFW/glfw3.h>

#include "../Window.h"

namespace Graphics
{
	class GLRenderDevice;
	class GLWindow : public Window
	{
		friend GLRenderDevice;
	private:
		void* _handle;
	public:
		virtual void SetTitle(const std::string& title) override;

		virtual bool ShouldClose() override;
		virtual void PollEvents() override;

		virtual void* GetNativeHandle() override;

		static void GLFWErrorCallback(int id, const char* description);
	};
}