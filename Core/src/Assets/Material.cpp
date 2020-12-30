#include "Core/Assets/Material.h"

namespace Core
{
	const RenderStateBlock RenderStateBlock::DefaultOpaque =
	{
		/* blendEnabled = */ false,
		/* depthTestEnabled = */ true,
		/* depthWrite = */ true
	};

	const RenderStateBlock RenderStateBlock::DefaultTransparent =
	{
		/* blendEnabled = */ true,
		/* depthTestEnabled = */ true,
		/* depthWrite = */ false
	};

	const PropertyBlock PropertyBlock::Empty = {};

	Shader* Material::GetVertexShader() const { return _vertexShader; }
	Shader* Material::GetFragmentShader() const { return _fragmentShader; }
	RenderStateBlock Material::GetRenderStateBlock() const { return _renderStateBlock; }
}