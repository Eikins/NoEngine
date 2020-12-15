#pragma once

#include <iostream>

#include "Vector3.h"

namespace Math
{
    struct Matrix4x4;

    struct Quaternion
    {
    private:
        float x, y, z, w;
        Quaternion(const float& x_, const float& y_, const float& z_, const float& w_) : x(x_), y(y_), z(z_), w(w_) {}
    public:
        // Constructors
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        Vector3 RightVector() const;
        Vector3 UpVector() const;
        Vector3 ForwardVector() const;

        // Quaternion Operations
        static Quaternion Normalize(const Quaternion& q);
        static Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float& t);

        // Factory
        static Quaternion AxisAngle(const Vector3& axis, const float& angle);
        static Quaternion Euler(const Vector3& angles);
        static Quaternion Euler(const float& x, const float& y, const float& z);

        // Constants
        static const Quaternion Identity;
        static const Quaternion Zero;

        // Vector Operators
        friend bool operator==(const Quaternion& a, const Quaternion& b);
        friend bool operator!=(const Quaternion& a, const Quaternion& b);

        friend Quaternion operator+(const Quaternion& a, const Quaternion& b);
        friend Quaternion operator-(const Quaternion& a, const Quaternion& b);
        friend Quaternion operator-(const Quaternion& a);
        
        friend Quaternion operator*(const float& a, const Quaternion& b);
        friend Quaternion operator*(const Quaternion& a, const float& b);
        friend Quaternion operator*(const Quaternion& a, const Quaternion& b);
        friend Quaternion& operator*=(Quaternion& a, const Quaternion& b);
        friend std::ostream& operator<<(std::ostream& out, const Quaternion& q);

        friend struct Matrix4x4;
    };



}