#pragma once

#include <vector>
#include <array>

#include "Transform.h"
#include "GameObject.h"
#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Components/ScriptedBehaviour.h"
#include "Layer.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class SceneEditors;
}
#endif

namespace Core
{
	struct SceneCache
	{
		uint32_t _cachesSize = 0;
		std::vector<Renderer*> _opaqueRenderers;
		std::vector<Renderer*> _transparentRenderers;

		SceneCache(uint32_t cachesSize);

		void AddRenderer(RenderingLayer layer, Renderer* renderer);
		void GetRenderers(RenderingLayer layers, std::vector<Renderer*> renderers);
	};

	struct ComponentLimit
	{
		ComponentType type;
		uint32_t limit;
	};

	class Scene
	{
#ifdef NoEngine_Editor
		friend class Editor::SceneEditors;
#endif
		friend class GameObject;
	private:
		uint32_t _maxGameObjects;
		std::string _name;
		std::vector<GameObject> _gameObjects;
		std::vector<Transform*> _rootTransforms;

		// Components
		std::vector<Camera> _cameras;
		std::vector<Renderer> _renderers;
		std::vector<ScriptedBehaviour> _behaviours;

		std::array<uint32_t, 3> _componentLimits;

		SceneCache _cache;
		Component& CreateComponent(ComponentType type);
	public:
		Scene(std::string name, uint32_t maxGameObjects = 1024, std::vector<ComponentLimit> limits = {});

		GameObject& CreateGameObject(std::string name, Transform* parent = nullptr);
		GameObject& GetGameObjectAtIndex(uint32_t index);

		void GetRenderers(RenderingLayer layers, std::vector<Renderer*>& renderers);
		void Update();
	};
}