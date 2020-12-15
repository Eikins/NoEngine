#include "Math/Matrix4x4.h"

namespace Math
{
#pragma region Constants
    const Matrix4x4 Matrix4x4::Identity = Matrix4x4();
#pragma endregion

#pragma region Constructors
    Matrix4x4::Matrix4x4(float values[16])
    {
        m_0_0 = values[0];
        m_0_1 = values[1];
        m_0_2 = values[2];
        m_0_3 = values[3];

        m_1_0 = values[4];
        m_1_1 = values[5];
        m_1_2 = values[6];
        m_1_3 = values[7];

        m_2_0 = values[8];
        m_2_1 = values[9];
        m_2_2 = values[10];
        m_2_3 = values[11];

        m_3_0 = values[12];
        m_3_1 = values[13];
        m_3_2 = values[14];
        m_3_3 = values[15];
    }
#pragma endregion

#pragma region Factory Methods
    // Adapted from Python Implementation : https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/math/matrix4.py
    // From Noe Masse

    Matrix4x4 Matrix4x4::Translation(const Vector3& translation)
    {
        return Matrix4x4(
            1.0f, 0.0f, 0.0f, translation.x,
            0.0f, 1.0f, 0.0f, translation.y,
            0.0f, 0.0f, 1.0f, translation.z,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    Matrix4x4 Matrix4x4::Rotation(const Quaternion& rotation)
    {
        Quaternion n = Quaternion::Normalize(rotation);
        
        float xx = n.x * n.x;
        float yy = n.y * n.y;
        float zz = n.z * n.z;
        float xy = n.x * n.y;
        float xz = n.x * n.z;
        float xw = n.x * n.w;
        float yz = n.y * n.z;
        float yw = n.y * n.w;
        float zw = n.z * n.w;

        return Matrix4x4(
            1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0,
            2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0,
            2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0,
            0, 0, 0, 1
        );
    }

    Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
    {
        return Matrix4x4(
            scale.x, 0.0f, 0.0f, 0.0f,
            0.0f, scale.y, 0.0f, 0.0f,
            0.0f, 0.0f, scale.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
    {
        Quaternion n = Quaternion::Normalize(rotation);

        float xx = n.x * n.x;
        float yy = n.y * n.y;
        float zz = n.z * n.z;
        float xy = n.x * n.y;
        float xz = n.x * n.z;
        float xw = n.x * n.w;
        float yz = n.y * n.z;
        float yw = n.y * n.w;
        float zw = n.z * n.w;

        return Matrix4x4(
            scale.x * (1 - 2 * (yy + zz)), scale.y * (2 * (xy - zw))    , scale.z * (2 * (xz + yw))    , translation.x,
            scale.x * (2 * (xy + zw))    , scale.y * (1 - 2 * (xx + zz)), scale.z * (2 * (yz - xw))    , translation.y,
            scale.x * (2 * (xz - yw))    , scale.y * (2 * (yz + xw))    , scale.z * (1 - 2 * (xx + yy)), translation.z,
            0, 0, 0, 1
        );
    }
#pragma endregion


#pragma region Methods
    Vector4 Matrix4x4::GetRow(int i)
    {
        if (i < 0 || i > 3) return Vector4::Zero;
        float *row = &m_0_0 + (4 * i);
        return Vector4(row[0], row[1], row[2], row[3]);
    }

    Vector4 Matrix4x4::GetColumn(int i)
    {
        if (i < 0 || i > 3) return Vector4::Zero;
        float *col = &m_0_0 + i;
        return Vector4(col[0], col[4], col[8], col[12]);
    }

    void Matrix4x4::SetRow(int i, const Vector4& values)
    {
        if (i < 0 || i > 3) return;
        float *row = &m_0_0 + (4 * i);
        row[0] = values.x;
        row[1] = values.y;
        row[2] = values.z;
        row[3] = values.w;
    }

    void Matrix4x4::SetColumn(int i, const Vector4& values)
    {
        if (i < 0 || i > 3) return;
        float *col = &m_0_0 + i;
        col[0] = values.x;
        col[4] = values.y;
        col[8] = values.z;
        col[12] = values.w;
    }
#pragma endregion

#pragma region Operators
    float& Matrix4x4::operator()(int row, int col)
    {
        if (row < 0 || row > 3 || col < 0 || col > 3) return m_0_0;
        return *(&m_0_0 + (4 * row) + col);
    }

    float Matrix4x4::operator()(int row, int col) const
    {
        if (row < 0 || row > 3 || col < 0 || col > 3) return m_0_0;
        return *(&m_0_0 + (4 * row) + col);
    }

    Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b) {
        return Matrix4x4(
            // Row 0
            a.m_0_0 * b.m_0_0 + a.m_0_1 * b.m_1_0 + a.m_0_2 * b.m_2_0 + a.m_0_3 * b.m_3_0,
            a.m_0_0 * b.m_0_1 + a.m_0_1 * b.m_1_1 + a.m_0_2 * b.m_2_1 + a.m_0_3 * b.m_3_1,
            a.m_0_0 * b.m_0_2 + a.m_0_1 * b.m_1_2 + a.m_0_2 * b.m_2_2 + a.m_0_3 * b.m_3_2,
            a.m_0_0 * b.m_0_3 + a.m_0_1 * b.m_1_3 + a.m_0_2 * b.m_2_3 + a.m_0_3 * b.m_3_3,
            // Row 1
            a.m_1_0 * b.m_0_0 + a.m_1_1 * b.m_1_0 + a.m_1_2 * b.m_2_0 + a.m_1_3 * b.m_3_0,
            a.m_1_0 * b.m_0_1 + a.m_1_1 * b.m_1_1 + a.m_1_2 * b.m_2_1 + a.m_1_3 * b.m_3_1,
            a.m_1_0 * b.m_0_2 + a.m_1_1 * b.m_1_2 + a.m_1_2 * b.m_2_2 + a.m_1_3 * b.m_3_2,
            a.m_1_0 * b.m_0_3 + a.m_1_1 * b.m_1_3 + a.m_1_2 * b.m_2_3 + a.m_1_3 * b.m_3_3,
            // Row 2
            a.m_2_0 * b.m_0_0 + a.m_2_1 * b.m_1_0 + a.m_2_2 * b.m_2_0 + a.m_2_3 * b.m_3_0,
            a.m_2_0 * b.m_0_1 + a.m_2_1 * b.m_1_1 + a.m_2_2 * b.m_2_1 + a.m_2_3 * b.m_3_1,
            a.m_2_0 * b.m_0_2 + a.m_2_1 * b.m_1_2 + a.m_2_2 * b.m_2_2 + a.m_2_3 * b.m_3_2,
            a.m_2_0 * b.m_0_3 + a.m_2_1 * b.m_1_3 + a.m_2_2 * b.m_2_3 + a.m_2_3 * b.m_3_3,
            // Row 3
            a.m_3_0 * b.m_0_0 + a.m_3_1 * b.m_1_0 + a.m_3_2 * b.m_2_0 + a.m_3_3 * b.m_3_0,
            a.m_3_0 * b.m_0_1 + a.m_3_1 * b.m_1_1 + a.m_3_2 * b.m_2_1 + a.m_3_3 * b.m_3_1,
            a.m_3_0 * b.m_0_2 + a.m_3_1 * b.m_1_2 + a.m_3_2 * b.m_2_2 + a.m_3_3 * b.m_3_2,
            a.m_3_0 * b.m_0_3 + a.m_3_1 * b.m_1_3 + a.m_3_2 * b.m_2_3 + a.m_3_3 * b.m_3_3
        );
    }

    Vector4 operator*(const Matrix4x4& a, const Vector4& b) {
        return Vector4(
            // Dot products between rows and vec4
            a.m_0_0 * b.x + a.m_0_1 * b.y + a.m_0_2 * b.z + a.m_0_3 * b.w,
            a.m_1_0 * b.x + a.m_1_1 * b.y + a.m_1_2 * b.z + a.m_1_3 * b.w,
            a.m_2_0 * b.x + a.m_2_1 * b.y + a.m_2_2 * b.z + a.m_2_3 * b.w,
            a.m_3_0 * b.x + a.m_3_1 * b.y + a.m_3_2 * b.z + a.m_3_3 * b.w
        );
    }

    Vector4 operator*(const Vector4& a, const Matrix4x4& b) {
        return Vector4(
            // Dot products between columns and vec4
            a.x * b.m_0_0 + a.y * b.m_1_0 + a.z * b.m_2_0 + a.w * b.m_3_0,
            a.x * b.m_0_1 + a.y * b.m_1_1 + a.z * b.m_2_1 + a.w * b.m_3_1,
            a.x * b.m_0_2 + a.y * b.m_1_2 + a.z * b.m_2_2 + a.w * b.m_3_2,
            a.x * b.m_0_3 + a.y * b.m_1_3 + a.z * b.m_2_3 + a.w * b.m_3_3
        );
    }

    std::ostream& operator<<(std::ostream& out, const Matrix4x4& m)
    {
        return out << "Matrix4x4" << std::endl
            << "|" << m.m_0_0 << ", " << m.m_0_1 << ", " << m.m_0_2 << ", " << m.m_0_3 << "|" << std::endl
            << "|" << m.m_1_0 << ", " << m.m_1_1 << ", " << m.m_1_2 << ", " << m.m_1_3 << "|" << std::endl
            << "|" << m.m_2_0 << ", " << m.m_2_1 << ", " << m.m_2_2 << ", " << m.m_2_3 << "|" << std::endl
            << "|" << m.m_3_0 << ", " << m.m_3_1 << ", " << m.m_3_2 << ", " << m.m_3_3 << "|" << std::endl;
    }
#pragma endregion
}