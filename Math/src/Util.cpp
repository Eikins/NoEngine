#include <math.h>
#include "Math/Util.h"

namespace Math
{
    float Max(const float& a, const float& b) { return a > b ? a : b; }
    float Min(const float& a, const float& b) { return a < b ? a : b; }
    float Clamp(const float& x, const float& min, const float& max) { return x > max ? max : (x < min ? min : x); }

    float Saturate(float x) { return x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x); }
    float Clip(float x) { return x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x); }

    float Cos(float x) { return cos(x); }
    float Sin(float x) { return sin(x); }
}