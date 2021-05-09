#pragma once

#include <iostream>
#include <math.h>

#include "Vector3.h"
#include "Quaternion.h"

namespace Math
{
    struct Pose
    {
    public:
        Vector3 position = Vector3::Zero;
        Quaternion rotation = Quaternion::Identity;

        // Constructors
        Pose(const Vector3& position_, const Quaternion& rotation_) : position(position_), rotation(rotation_) {}
        Pose() : position(Vector3::Zero), rotation(Quaternion::Identity) {}
    };
}