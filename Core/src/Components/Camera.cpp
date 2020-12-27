#include "Core/Components/Camera.h"

namespace Core
{
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
		}
		return _projectionMatrix;
	}
}