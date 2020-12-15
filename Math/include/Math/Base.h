#pragma once

#include <math.h>

namespace Math
{
    const float PI = 3.14159265358979323846f;
    const float TAU = 2.0f * PI;

    const float Deg2Rad = PI / 180.0f;
    const float Rad2Deg = 180.0f / PI;

    float Max(const float& a, const float& b) { return a > b ? a : b; }
    float Min(const float& a, const float& b) { return a < b ? a : b; }
    float Clamp(const float& x, const float& min, const float& max) { return x > max ? max : (x < min ? min : x); }

    float Saturate(float x) { return x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x); } // Equivalent tu Clamp(x, 0, 1)
    float Clip(float x) { return x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x); }   // Equivalent tu Clamp(x, -1, 1)

    float Cos(float x) { return cos(x); }
    float Sin(float x) { return sin(x); }
}