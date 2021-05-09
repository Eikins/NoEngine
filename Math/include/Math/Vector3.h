#pragma once

#include <iostream>

namespace Math
{
    struct Vector3
    {
    public:
        float x, y, z;

        // Constructors
        Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
        Vector3(float x_, float y_) : x(x_), y(y_), z(0.0f) {}
        Vector3(float x_) : x(x_), y(0.0f), z(0.0f) {}
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

        // Vector Operations
        static float Sum(const Vector3& a);
        static float Dot(const Vector3& a, const Vector3& b);
        static float LengthSquared(const Vector3& a);
        static float Length(const Vector3& a);

        static Vector3 Normalize(const Vector3& a);
        static Vector3 Cross(const Vector3& a, const Vector3& b);

        // Constants
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Right;
        static const Vector3 Up;
        static const Vector3 Forward;
    };

    // Vector Operators
    bool operator==(const Vector3& a, const Vector3& b);
    bool operator!=(const Vector3& a, const Vector3& b);

    Vector3 operator+(const Vector3& a, const Vector3& b);
    Vector3 operator-(const Vector3& a, const Vector3& b);
    Vector3 operator-(const Vector3& a);

    Vector3 operator*(const float& a, const Vector3& b);
    Vector3 operator*(const Vector3& a, const float& b);
    Vector3 operator*(const Vector3& a, const Vector3& b);

    Vector3 operator/(const Vector3& a, const float& b);

    Vector3& operator+=(Vector3& a, const Vector3& b);

    bool operator<(const Vector3& a, const Vector3& b);
    bool operator>(const Vector3& a, const Vector3& b);
    bool operator<=(const Vector3& a, const Vector3& b);
    bool operator>=(const Vector3& a, const Vector3& b);

    std::ostream& operator<<(std::ostream& out, const Vector3& v);
}