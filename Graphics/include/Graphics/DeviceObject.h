#pragma once

#include <string>

namespace Graphics
{
	class RenderDevice;
	class DeviceObject
	{
		friend RenderDevice;
	protected:
		RenderDevice* _device = nullptr;
	public:
		virtual void Release() = 0;
		virtual void* GetNativeHandle() const = 0;
	};
}