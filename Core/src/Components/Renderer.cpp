#include "Core/Components/Renderer.h"

namespace Core
{
	bool Renderer::operator==(const Renderer& rhs) const
	{
		return mesh == rhs.mesh && material == rhs.material;
	}
}