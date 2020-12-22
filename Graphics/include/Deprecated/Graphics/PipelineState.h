#pragma once

#include "Shader.h"
#include "SwapChain.h"

namespace Graphics
{
	enum class PolygonMode { FILL, LINE, POINT };

	enum class CullMode { DISABLED, FRONT, BACK, FRONT_AND_BACK };

	enum class WindingOrder { CLOCKWISE, COUNTER_CLOCKWISE };

	struct BlendStateDescriptor
	{
		bool enabled = false;
	};

	struct RasterizerStateDescriptor
	{
		PolygonMode polygonMode = PolygonMode::FILL;
		bool clampDepth = false;
		WindingOrder windingOrder = WindingOrder::CLOCKWISE;
		CullMode cullMode = CullMode::BACK;
	};

	struct DepthStencilStateDescriptor
	{

	};

	struct Viewport
	{
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float minDepth = 0.0f;
		float maxDepth = 1.0f;
	};

	struct PipelineStateCreationDescriptor
	{
		SwapChain* pSwapChain; // TODO: Needs to be removed in the future...
		Shader* pVertexShader;
		Shader* pPixelShader;
		std::string vertexEntrypoint;
		std::string pixelEntrypoint;
		Viewport viewport;
		BlendStateDescriptor blendState;
		RasterizerStateDescriptor rasterizerState;
		DepthStencilStateDescriptor depthStencilState;
	};

	class PipelineState : public DeviceObject
	{
	public:
	};
}