#pragma once

#include "Math/Math.h"

#include "Component.h"

namespace Editor
{
	class CameraEditor;
}

namespace Core
{
	class Camera : public Component
	{
	public:
		friend class Editor::CameraEditor;
	private:
		float _fieldOfView = 90.0f;
		float _aspectRatio = 1.0f;
		float _nearPlane = 0.3f;
		float _farPlane = 1000.0f;

		bool _hasChanged = true;
		Math::Matrix4x4 _projectionMatrix;
	public:
		Math::Matrix4x4 GetProjectionMatrix();
	};
}