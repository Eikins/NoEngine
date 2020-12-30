#include "Core/Scene.h"

namespace Core
{
#pragma region Scene Cache
	SceneCache::SceneCache(uint32_t cachesSize)
	{
		_cachesSize = cachesSize;
		_opaqueRenderers.reserve(cachesSize);
		_transparentRenderers.reserve(cachesSize);
	}

	void SceneCache::AddRenderer(RenderingLayer layer, Renderer* renderer)
	{
		if (renderer == nullptr) return;

		if ((layer & RenderingLayer::Opaque) == RenderingLayer::Opaque) _opaqueRenderers.push_back(renderer);
		if ((layer & RenderingLayer::Transparent) == RenderingLayer::Transparent) _transparentRenderers.push_back(renderer);
	}

	void SceneCache::GetRenderers(RenderingLayer layers, std::vector<Renderer*> renderers)
	{
		renderers.clear();
		uint32_t size = 0;
		if ((layers & RenderingLayer::Opaque) == RenderingLayer::Opaque) size += _opaqueRenderers.size();
		if ((layers & RenderingLayer::Transparent) == RenderingLayer::Transparent) size += _transparentRenderers.size();
	
		renderers.reserve(size);
		if ((layers & RenderingLayer::Opaque) == RenderingLayer::Opaque)
		{
			renderers.insert(renderers.end(), _opaqueRenderers.begin(), _opaqueRenderers.end());
		}
		if ((layers & RenderingLayer::Transparent) == RenderingLayer::Transparent)
		{
			renderers.insert(renderers.end(), _transparentRenderers.begin(), _transparentRenderers.end());
		}
	}
#pragma endregion

#pragma region Scene
	Scene::Scene(std::string name, uint32_t maxGameObjects, std::vector<ComponentLimit> customLimits) : _cache(maxGameObjects)
	{
		_name = name;
		_maxGameObjects = maxGameObjects;
		_gameObjects.reserve(maxGameObjects);

		// Reserve components memory arrays
		// We want to have all components stored in contiguous arrays to avoid cache miss
		// Default limits
		_componentLimits =
		{
			4, // camera
			maxGameObjects // renderers
		};

		for (uint32_t i = 0; i < customLimits.size(); i++)
		{
			_componentLimits[static_cast<uint32_t>(customLimits[i].type)] = customLimits[i].limit;
		}

		_cameras.reserve(_componentLimits[static_cast<uint32_t>(ComponentType::Camera)]);
		_renderers.reserve(_componentLimits[static_cast<uint32_t>(ComponentType::Renderer)]);
	}

	GameObject& Scene::GetGameObjectAtIndex(uint32_t index)
	{
		return _gameObjects[index];
	}

	GameObject& Scene::CreateGameObject(std::string name, Transform* parent)
	{
		uint32_t index = _gameObjects.size();
		if (index >= _maxGameObjects)
		{
			throw std::runtime_error("Couldn't create another game object ! Scene object limit exceeded.");
		}

		_gameObjects.push_back(GameObject());
		GameObject& gameObject = _gameObjects[index];
		gameObject._scene = this;
		gameObject.sceneIndex = index;
		gameObject._name = name;
		gameObject._transform._gameObject = &gameObject;

		if (parent == nullptr)
		{
			_rootTransforms.push_back(&(gameObject._transform));
		}
		else
		{
			gameObject._transform.SetParent(parent);
		}

		return _gameObjects[index];
	}

	void Scene::GetRenderers(RenderingLayer layers, std::vector<Renderer*>& renderers)
	{
		renderers.clear();
		renderers.reserve(_renderers.size());
		for (uint32_t i = 0; i < _renderers.size(); i++)
		{
			renderers.push_back(&_renderers[i]);
		}
	}

	Component& Scene::CreateComponent(ComponentType type)
	{
		int index = -1;
		switch (type)
		{
		case (ComponentType::Camera):
			index = _cameras.size();
			break;
		case (ComponentType::Renderer):
			index = _renderers.size();
			break;
		}

		uint32_t limit = _componentLimits[static_cast<uint32_t>(type)];
		if (index >= limit)
		{
			throw std::runtime_error("Couldn't create another component ! Limit exceeded.");
		}

		switch (type)
		{
		case (ComponentType::Camera):
			_cameras.push_back(Camera());
			return _cameras[index];
		case (ComponentType::Renderer):
			_renderers.push_back(Renderer(nullptr, nullptr));
			return _renderers[index];
		}
	}
#pragma endregion


}