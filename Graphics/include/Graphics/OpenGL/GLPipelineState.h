#pragma once

#include "Graphics/PipelineState.h"

namespace Graphics
{
	class GLPipelineState : public PipelineState
	{
	public:
		uint32_t GLID;

		void Bind();

		virtual void Release() override;
		virtual void* GetNativeHandle() override;
	};
}