#pragma once

#include <string>

namespace Graphics
{
	struct WindowDescriptor
	{
		std::string title = "Untitled";
		uint32_t width = 800;
		uint32_t height = 600;
		bool resizable = false;
	};

	class Window
	{
	public:

		static std::unique_ptr<Window> Create(const WindowDescriptor &descriptor);

		virtual void SetTitle(const std::string &title) = 0;
		virtual std::string GetTitle() const = 0;
	};
}