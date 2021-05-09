#pragma once


#include "DeviceObject.h"
#include "RenderPass.h"

namespace Graphics
{

	struct FramebufferDescriptor
	{
		RenderPass* renderPass = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	class Framebuffer : public DeviceObject
	{
	public:
	};
}