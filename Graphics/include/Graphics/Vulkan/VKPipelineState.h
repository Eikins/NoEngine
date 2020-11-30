#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/PipelineState.h"

namespace Graphics
{
	class VKRenderDevice;
	class VKPipelineState : public PipelineState
	{
		friend VKRenderDevice;
	private:
		VkRenderPass _renderPass;
		VkPipelineLayout _layout;
		VkPipeline _pipeline;
	public:
		virtual void Release() override;
		virtual void* GetNativeHandle() override;
	};
}