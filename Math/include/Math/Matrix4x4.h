#pragma once

#include <iostream>

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace Math
{
    struct Matrix4x4
    {
    public:
        float m_0_0, m_0_1, m_0_2, m_0_3;
        float m_1_0, m_1_1, m_1_2, m_1_3;
        float m_2_0, m_2_1, m_2_2, m_2_3;
        float m_3_0, m_3_1, m_3_2, m_3_3;

        // Constructors
        Matrix4x4() :
            m_0_0(1), m_0_1(0), m_0_2(0), m_0_3(0),
            m_1_0(0), m_1_1(1), m_1_2(0), m_1_3(0),
            m_2_0(0), m_2_1(0), m_2_2(1), m_2_3(0),
            m_3_0(0), m_3_1(0), m_3_2(0), m_3_3(1) {}

        Matrix4x4(float values[16]);
        Matrix4x4(float m_0_0_, float m_0_1_, float m_0_2_, float m_0_3_,
            float m_1_0_, float m_1_1_, float m_1_2_, float m_1_3_,
            float m_2_0_, float m_2_1_, float m_2_2_, float m_2_3_,
            float m_3_0_, float m_3_1_, float m_3_2_, float m_3_3_) :
            m_0_0(m_0_0_), m_0_1(m_0_1_), m_0_2(m_0_2_), m_0_3(m_0_3_),
            m_1_0(m_1_0_), m_1_1(m_1_1_), m_1_2(m_1_2_), m_1_3(m_1_3_),
            m_2_0(m_2_0_), m_2_1(m_2_1_), m_2_2(m_2_2_), m_2_3(m_2_3_),
            m_3_0(m_3_0_), m_3_1(m_3_1_), m_3_2(m_3_2_), m_3_3(m_3_3_) {}

        // Methods
        Vector4 GetRow(int i);
        Vector4 GetColumn(int i);

        void SetRow(int i, const Vector4& values);
        void SetColumn(int i, const Vector4& values);

        // Value accessors, prefer using direct access to avoid overhead
        // Useful for prototyping algorithms
        float& operator()(int row, int col);
        float operator()(int row, int col) const;

        // Factory
        static Matrix4x4 Translation(const Vector3& translation);
        static Matrix4x4 Rotation(const Quaternion& rotation);
        static Matrix4x4 Scale(const Vector3& scale);

        static Matrix4x4 TRS(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);

        static const Matrix4x4 Identity;
    };

    Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
    Vector4 operator*(const Vector4& v, const Matrix4x4& m);
    Vector4 operator*(const Matrix4x4& m, const Vector4& v);

    std::ostream& operator<<(std::ostream& out, const Matrix4x4& m);
}