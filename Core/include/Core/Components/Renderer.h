#pragma once

#include "Component.h"
#include "Core/Assets/Material.h"
#include "Core/Assets/Mesh.h"

#ifdef NoEngine_Editor
class Editor::ComponentEditors;
#endif

namespace Core
{
	class Renderer : public Component
	{
#ifdef NoEngine_Editor
		friend class Editor::ComponentEditors;
#endif
	public:
		Mesh* mesh = nullptr;
		Material* material = nullptr;

		Renderer(Mesh* mesh, Material* material) : Component(ComponentType::Renderer) {}

		bool operator==(const Renderer& rhs) const;
	};
}