#pragma once

#include <iostream>
#include <math.h>

#include "Vector3.h"

namespace Math
{
    struct Vector4
    {
    public:
        float x, y, z, w;

        // Constructors
        Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
        Vector4(float x_, float y_, float z_) : x(x_), y(y_), z(z_), w(1.0f) {}
        Vector4(float x_, float y_) : x(x_), y(y_), z(0.0f), w(1.0f) {}
        Vector4(float x_) : x(x_), y(0.0f), z(0.0f), w(1.0f) {}
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        Vector4(const Vector3& v_, float w_) : x(v_.x), y(v_.y), z(v_.z), w(w_) {}

        // Constants
        static const Vector4 Zero;

        operator Vector3() const;
    };

    // Vector Operators
    bool operator==(const Vector4& a, const Vector4& b);

    Vector4 operator+(const Vector4& a, const Vector4& b);
    Vector4 operator-(const Vector4& a, const Vector4& b);
    Vector4 operator-(const Vector4& a);

    Vector4 operator*(const float& a, const Vector4& b);
    Vector4 operator*(const Vector4& a, const float& b);
    Vector4 operator*(const Vector4& a, const Vector4& b);

    Vector4 operator/(const Vector4& a, const float& b);

    Vector4& operator+=(Vector4& a, const Vector4& b);

    std::ostream& operator<<(std::ostream& out, const Vector4& v);
}