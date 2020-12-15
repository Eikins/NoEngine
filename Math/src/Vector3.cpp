#include <math.h>

#include "Math/Vector3.h"

namespace Math
{
    float Vector3::Sum(const Vector3& a) { return a.x + a.y + a.z; }
    float Vector3::Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    float Vector3::LengthSquared(const Vector3& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
    float Vector3::Length(const Vector3& a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

    Vector3 Vector3::Normalize(const Vector3&a)
    {
        if (a == Vector3::Zero)
        {
            return a;
        }
        else
        {
            return a / Vector3::Length(a);
        }
    }

    Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    const Vector3 Vector3::Zero = Vector3();
    const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::Right = Vector3(1.0f);
    const Vector3 Vector3::Up = Vector3(0.0f, 1.0f);
    const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);

    // Vector Operators
    bool operator==(const Vector3& a, const Vector3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
    bool operator!=(const Vector3& a, const Vector3& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

    Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
    Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
    Vector3 operator-(const Vector3& a) { return Vector3(-a.x, -a.y, -a.z); }

    Vector3 operator*(const float& a, const Vector3& b) { return Vector3(a * b.x, a * b.y, a * b.z); }
    Vector3 operator*(const Vector3& a, const float& b) { return Vector3(a.x * b, a.y * b, a.z * b); }
    Vector3 operator*(const Vector3& a, const Vector3& b) { return Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }

    Vector3 operator/(const Vector3& a, const float& b) { return Vector3(a.x / b, a.y / b, a.z / b); }

    Vector3& operator+=(Vector3& a, const Vector3& b)
    { 
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    std::ostream& operator<<(std::ostream& out, const Vector3& v) { return out << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")"; }
}