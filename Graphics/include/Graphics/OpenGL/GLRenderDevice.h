#pragma once

#include "../RenderDevice.h"

namespace Graphics
{
	class GLRenderDevice : public RenderDevice
	{
		virtual Buffer CreateBuffer(BufferDescriptor descriptor, BufferData data) override;
	};
}