#pragma once

#include <iostream>

namespace Math
{
    struct Vector2
    {
    public:
        float x, y;

        // Constructors
        Vector2(float x_, float y_) : x(x_), y(y_) {}
        Vector2(float x_) : x(x_), y(0.0f) {}
        Vector2() : x(0.0f), y(0.0f) {}

        // Vector Operations
        static float Sum(const Vector2& a);
        static float Dot(const Vector2& a, const Vector2& b);
        static float LengthSquared(const Vector2& a);
        static float Length(const Vector2& a);

        static Vector2 Normalize(const Vector2& a);

        // Constants
        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 Right;
        static const Vector2 Up;
    };

    // Vector Operators
    bool operator==(const Vector2& a, const Vector2& b);

    Vector2 operator+(const Vector2& a, const Vector2& b);
    Vector2 operator-(const Vector2& a, const Vector2& b);
    Vector2 operator-(const Vector2& a);

    Vector2 operator*(const float& a, const Vector2& b);
    Vector2 operator*(const Vector2& a, const float& b);
    Vector2 operator*(const Vector2& a, const Vector2& b);

    Vector2 operator/(const Vector2& a, const float& b);

    Vector2& operator+=(Vector2& a, const Vector2& b);

    std::ostream& operator<<(std::ostream& out, const Vector2& v);
}