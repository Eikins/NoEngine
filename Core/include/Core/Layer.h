#pragma once

#include <cstdint>

namespace Core
{
	enum class RenderingLayer : uint8_t
	{
		None = 0,
		Opaque = 1 << 0,
		Transparent = 1 << 1
	};

	RenderingLayer operator|(RenderingLayer lhs, RenderingLayer rhs);
	RenderingLayer operator&(const RenderingLayer& lhs, RenderingLayer rhs);

	enum class PhysicLayer : uint32_t
	{
		Default = 1 << 0
	};
}