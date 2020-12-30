#include "Core/Components/Renderer.h"

namespace Core
{
	bool Renderer::operator==(const Renderer& rhs) const
	{
		return _transform == rhs._transform && mesh == rhs.mesh && material == rhs.material;
	}
}