#pragma once

#include <math.h>

namespace Math
{
    const float PI = 3.14159265358979323846f;
    const float TAU = 2.0f * PI;

    const float Deg2Rad = PI / 180.0f;
    const float Rad2Deg = 180.0f / PI;

    inline float Max(const float& a, const float& b) { return a > b ? a : b; }
    inline float Min(const float& a, const float& b) { return a < b ? a : b; }
    inline float Clamp(const float& x, const float& min, const float& max) { return x > max ? max : (x < min ? min : x); }

    inline float Abs(const float& x) { return x < 0 ? -x : x; }

    inline float Saturate(float x) { return x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x); }
    inline float Clip(float x) { return x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x); }
 
    inline float Cos(float x) { return cos(x); }
    inline float Sin(float x) { return sin(x); }
}