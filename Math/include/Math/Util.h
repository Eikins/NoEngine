#pragma once

namespace Math
{
    const float PI = 3.14159265358979323846f;
    const float TAU = 2.0f * PI;

    const float Deg2Rad = PI / 180.0f;
    const float Rad2Deg = 180.0f / PI;

    float Max(const float& a, const float& b);
    float Min(const float& a, const float& b);
    float Clamp(const float& x, const float& min, const float& max);

    float Saturate(float x);
    float Clip(float x);

    float Cos(float x);
    float Sin(float x);
}