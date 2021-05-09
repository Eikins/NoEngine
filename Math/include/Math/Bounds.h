#pragma once

#include <iostream>
#include <math.h>

#include "Vector3.h"

namespace Math
{
    struct Bounds
    {
    public:
        Vector3 min;
        Vector3 max;

        // Constructors
        Bounds(const Vector3& center, const Vector3& size);
        Bounds() : min(Vector3::Zero), max(Vector3::Zero) {}

        inline bool Contains(const Vector3& position) const { return (min <= position) && (position <= max); }
        inline bool Overlaps(const Bounds& other) const { return min <= other.max && max >= other.min; }

        inline Vector3 Center() const { return (min + max) / 2; }
        inline Vector3 Size() const { return max - min; }

        inline Bounds Translate(const Vector3& translation) {
            Bounds bounds;
            bounds.min = min + translation;
            bounds.max = max + translation;
            return bounds;
        }
    };
}