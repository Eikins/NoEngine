#include "Core/Layer.h"

namespace Core
{
	RenderingLayer operator|(RenderingLayer lhs, RenderingLayer rhs)
	{
		return static_cast<RenderingLayer>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	RenderingLayer operator&(const RenderingLayer& lhs, RenderingLayer rhs)
	{
		return static_cast<RenderingLayer>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}
}