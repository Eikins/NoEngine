#include "Core/Components/Camera.h"

namespace Core
{
	void Camera::SetAspectRatio(float aspectRatio)
	{
		_aspectRatio = aspectRatio;
		_hasChanged = true;
	}

	Math::Matrix4x4 Camera::GetProjectionMatrix()
	{
		if (_hasChanged)
		{
			_projectionMatrix = Math::Matrix4x4::PerspectiveZ01(
				_fieldOfView,
				_aspectRatio,
				_nearPlane,
				_farPlane
			);
			_hasChanged = false;
		}
		return _projectionMatrix;
	}
}