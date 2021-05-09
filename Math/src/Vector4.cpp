#include <math.h>

#include "Math/Vector4.h"

namespace Math
{
    const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

    Vector4::operator Vector3() const
    {
        return Vector3(x, y, z);
    }

    // Vector Operators
    bool operator==(const Vector4& a, const Vector4& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

    Vector4 operator+(const Vector4& a, const Vector4& b) { return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    Vector4 operator-(const Vector4& a, const Vector4& b) { return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    Vector4 operator-(const Vector4& a) { return Vector4(-a.x, -a.y, -a.z, -a.w); }

    Vector4 operator*(const float& a, const Vector4& b) { return Vector4(a * b.x, a * b.y, a * b.z, a * b.w); }
    Vector4 operator*(const Vector4& a, const float& b) { return Vector4(a.x * b, a.y * b, a.z * b, a.w * b); }
    Vector4 operator*(const Vector4& a, const Vector4& b) { return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }

    Vector4 operator/(const Vector4& a, const float& b) { return Vector4(a.x / b, a.y / b, a.z / b, a.w / b); }

    Vector4& operator+=(Vector4& a, const Vector4& b)
    { 
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
        return a;
    }

    std::ostream& operator<<(std::ostream& out, const Vector4& v) { return out << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; }
}