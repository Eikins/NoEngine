#include <math.h>

#include "Math/Vector2.h"

namespace Math
{
    float Vector2::Sum(const Vector2& a) { return a.x + a.y; }
    float Vector2::Dot(const Vector2& a, const Vector2& b) { return a.x * b.x + a.y * b.y; }
    float Vector2::LengthSquared(const Vector2& a) { return a.x * a.x + a.y * a.y; }
    float Vector2::Length(const Vector2& a) { return sqrt(a.x * a.x + a.y * a.y); }

    Vector2 Vector2::Normalize(const Vector2&a)
    {
        if (a == Vector2::Zero)
        {
            return a;
        }
        else
        {
            return a / Vector2::Length(a);
        }
    }

    const Vector2 Vector2::Zero = Vector2();
    const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
    const Vector2 Vector2::Right = Vector2(1.0f);
    const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);

    // Vector Operators
    bool operator==(const Vector2& a, const Vector2& b) { return a.x == b.x && a.y == b.y; }

    Vector2 operator+(const Vector2& a, const Vector2& b) { return Vector2(a.x + b.x, a.y + b.y); }
    Vector2 operator-(const Vector2& a, const Vector2& b) { return Vector2(a.x - b.x, a.y - b.y); }
    Vector2 operator-(const Vector2& a) { return Vector2(-a.x, -a.y); }

    Vector2 operator*(const float& a, const Vector2& b) { return Vector2(a * b.x, a * b.y); }
    Vector2 operator*(const Vector2& a, const float& b) { return Vector2(a.x * b, a.y * b); }
    Vector2 operator*(const Vector2& a, const Vector2& b) { return Vector2(a.x * b.x, a.y * b.y); }

    Vector2 operator/(const Vector2& a, const float& b) { return Vector2(a.x / b, a.y / b); }

    Vector2& operator+=(Vector2& a, const Vector2& b)
    { 
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    std::ostream& operator<<(std::ostream& out, const Vector2& v) { return out << "Vector2(" << v.x << ", " << v.y << ")"; }
}