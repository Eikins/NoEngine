#pragma once

#include <iostream>
#include <math.h>

#include "Vector2.h"

namespace Math
{
    struct Rect
    {
    public:
        Vector2 position;
        Vector2 size;

        // Constructors
        Rect(const Vector2& position_, const Vector2& size_) : position(position_), size(size_) {}
        Rect() : position(Vector2::Zero), size(Vector2::One) {}
    };

    std::ostream& operator<<(std::ostream& out, const Rect& rect);
}