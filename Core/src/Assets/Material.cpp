#include "Core/Assets/Material.h"

#include <cassert>

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
		/* depthTestEnabled = */ false,
		/* depthWrite = */ false
	};

	const PropertyBlock PropertyBlock::Empty = PropertyBlock({});

	PropertyBlock::PropertyBlock(const std::vector<MaterialPropertyDescriptor>& properties)
	{
		_properties.resize(properties.size());
		_nameToProperty.reserve(properties.size());
		for (size_t i = 0; i < properties.size(); i++)
		{
			auto& prop = properties[i];
			_properties[i].type = prop.type;
			assert(_nameToProperty.find(prop.name) == _nameToProperty.end() && "Duplicate material property found.");
			_nameToProperty.insert({ prop.name, i });
		}
		if (properties.size() > 0) _hasChanged = true;
	}

	PropertyBlock::PropertyBlock(const PropertyBlock& propertyBlock)
	{
		_properties = std::vector<MaterialProperty>(propertyBlock._properties);
		_nameToProperty = std::unordered_map<std::string, uint32_t>(propertyBlock._nameToProperty);
		_hasChanged = _properties.size() > 0;
	}

	void PropertyBlock::SetInt(std::string name, uint32_t value)
	{
		auto it = _nameToProperty.find(name);
		if (it != _nameToProperty.end())
		{
			_properties[it->second].intValue = value;
		}
	}

	void PropertyBlock::SetVector(std::string name, Math::Vector4 value)
	{
		auto it = _nameToProperty.find(name);
		if (it != _nameToProperty.end())
		{
			_properties[it->second].vectorValue = value;
		}
	}

	Shader* Material::GetVertexShader() const { return _vertexShader; }
	Shader* Material::GetFragmentShader() const { return _fragmentShader; }
	RenderStateBlock Material::GetRenderStateBlock() const { return _renderStateBlock; }
	PropertyBlock& Material::GetPropertyBlock() { return _propertyBlock; }
}