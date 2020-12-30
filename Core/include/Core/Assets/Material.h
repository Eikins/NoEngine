#pragma once

#include "Math/Math.h"
#include "Asset.h"
#include "Shader.h"

namespace Core
{
	struct RenderStateBlock
	{
		bool blendEnabled = false;
		bool depthTestEnabled = true;
		bool depthWrite = true;

		static const RenderStateBlock DefaultOpaque;
		static const RenderStateBlock DefaultTransparent;
	};

	struct PropertyBlock
	{
		struct IntProperty { uint32_t binding; uint32_t value; };
		struct Vector4Property { uint32_t binding; Math::Vector4 value; };
		// struct TextureProperty { uint32_t binding; Texture value; };

		static const PropertyBlock Empty;
	};

	class Material : public Asset
	{
	private:
		Shader* _vertexShader;
		Shader* _fragmentShader;
		RenderStateBlock _renderStateBlock;
		
	public:
		Material(
			std::string name,
			Shader* vertexShader, 
			Shader* fragmentShader,
			RenderStateBlock renderStateBlock = RenderStateBlock::DefaultOpaque,
			PropertyBlock propertyBlock = PropertyBlock::Empty
		) : _vertexShader(vertexShader),
			_fragmentShader(fragmentShader),
			_renderStateBlock(renderStateBlock),
			Asset(name) {}

		Shader* GetVertexShader() const;
		Shader* GetFragmentShader() const;
		RenderStateBlock GetRenderStateBlock() const;
	};
}