#pragma once
#include <unordered_map>
#include <string>

#include "Math/Math.h"
#include "Asset.h"
#include "Shader.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class GameObjectEditor;
}
#endif

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

	enum class MaterialPropertyType : uint8_t
	{
		INT = 0,
		TEXTURE = 1,
		FLOAT = 2,
		VECTOR2 = 3,
		VECTOR3 = 4,
		VECTOR4 = 5,
		COLOR = 6
	};

	struct MaterialPropertyDescriptor
	{
		std::string name;
		MaterialPropertyType type;

		MaterialPropertyDescriptor(std::string name_, MaterialPropertyType type_) : name(name_), type(type_) {}
	};

	struct MaterialProperty
	{
		MaterialPropertyType type;
		// Int value is used for int and texture values
		uint32_t intValue = 0;
		// float and vectors are stored in the vector value
		Math::Vector4 vectorValue = Math::Vector4::Zero;
	};

	class PropertyBlock
	{
#ifdef NoEngine_Editor
		friend class Editor::GameObjectEditor;
#endif
	private:
		std::unordered_map<std::string, uint32_t> _nameToProperty;
		std::vector<MaterialProperty> _properties;
		bool _hasChanged = false;
	public:
		PropertyBlock(const std::vector<MaterialPropertyDescriptor>& properties);
		PropertyBlock(const PropertyBlock& propertBlock);

		void SetInt(std::string name, uint32_t value);
		void SetVector(std::string name, Math::Vector4 value);

		inline bool HasChanged() const { return _hasChanged; }
		inline void MarkUpToDate() { _hasChanged = false; }

		inline std::vector<MaterialProperty>& Properties() { return _properties; }

		static const PropertyBlock Empty;
	};

	class Material : public Asset
	{
	private:
		Shader* _vertexShader;
		Shader* _fragmentShader;
		RenderStateBlock _renderStateBlock;
		PropertyBlock _propertyBlock;
		bool _isShared = false;
	public:
		Material(
			std::string name,
			Shader* vertexShader, 
			Shader* fragmentShader,
			RenderStateBlock renderStateBlock = RenderStateBlock::DefaultOpaque,
			const std::vector<MaterialPropertyDescriptor>& properties = {},
			bool isShared = true
		) : _vertexShader(vertexShader),
			_fragmentShader(fragmentShader),
			_renderStateBlock(renderStateBlock),
			_propertyBlock(properties),
			_isShared(isShared),
			Asset(name) {}

		Material(const Material& material) :
			_vertexShader(material._vertexShader),
			_fragmentShader(material._fragmentShader),
			_renderStateBlock(material._renderStateBlock),
			_propertyBlock(material._propertyBlock),
			_isShared(false),
			Asset(material.GetName() + " (Instance)") {}

		Shader* GetVertexShader() const;
		Shader* GetFragmentShader() const;
		RenderStateBlock GetRenderStateBlock() const;
		PropertyBlock& GetPropertyBlock();

		inline bool IsShared() const { return _isShared; }
	};
}