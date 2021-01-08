#include "Math/Bounds.h"

#include "Math/Util.h"

namespace Math
{
	Bounds::Bounds(const Vector3& center_, const Vector3& size_)
	{
		auto min_ = center_ - size_ / 2;
		auto max_ = center_ + size_ / 2;
		min.x = Min(min_.x, max_.x);
		min.y = Min(min_.y, max_.y);
		min.z = Min(min_.z, max_.z);

		max.x = Max(min_.x, max_.x);
		max.y = Max(min_.y, max_.y);
		max.z = Max(min_.z, max_.z);
	}

}