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
    protected:
        WindowDescriptor _descriptor;
    public:
        virtual void SetTitle(const std::string &title) = 0;

        virtual bool ShouldClose() = 0;
        virtual void PollEvents() = 0;

        WindowDescriptor GetDescriptor() { return _descriptor; }
    };
}