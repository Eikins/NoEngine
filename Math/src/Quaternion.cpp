#include <math.h>

#include "Math/Util.h"
#include "Math/Quaternion.h"

namespace Math
{
    // Adapted from Python Implementation : https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/math/quaternion.py
    // From Noe Masse

    const Quaternion Quaternion::Identity = Quaternion();
    const Quaternion Quaternion::Zero = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

#pragma region Methods
    Vector3 Quaternion::RightVector() const
    {
        // Equivalent to Matrix.Quaternion(this) * Vector3::Right
        return Vector3(
            1 - 2 * (y * y + x * x),
            2 * (x * y + z * w),
            2 * (x * z - y * w)
        );
    }

    Vector3 Quaternion::UpVector() const
    {
        // Equivalent to Matrix.Quaternion(this) * Vector3::Right
        return Vector3(
            2 * (x * y - z * w),
            1 - 2 * (x * x + z * z),
            2 * (y * z + x * w)
        );
    }

    Vector3 Quaternion::ForwardVector() const
    {
        // Equivalent to Matrix.Quaternion(this) * Vector3::Right
        return Vector3(
            2 * (x * z + y * w),
            2 * (y * z - x * w),
            1 - 2 * (x * x + y * y)
        );
    }
#pragma endregion

#pragma region Static Methods
    Quaternion Quaternion::Normalize(const Quaternion& q)
    {
        if (q == Quaternion::Zero)
        {
            return q;
        }
        else
        {
            float mag = sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
            return Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
        }
    }

    Quaternion Quaternion::Slerp(const Quaternion& start, const Quaternion& end, const float& t)
    {
        // Only unit quaternions are valid rotations.
        // Normalize to avoid undefined behavior.
        Quaternion q0 = Quaternion::Normalize(start);
        Quaternion q1 = Quaternion::Normalize(end);

        // Compute dot product.
        float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

        // If the dot product is negative, slerp won't take
        // the shorter path. Note that end and -end are equivalent when
        // the negation is applied to all four components. Fix by 
        // reversing one quaternion.

        if (dot < 0)
        {
            q1 = -q1;
            dot = -dot;
        }

        // Angle between input vectors
        float theta0 = acos(Clip(dot));
        // Angle between q0 and result
        float theta = theta0 * t;
        // Orthogonalize the basis
        q1 = Quaternion::Normalize(q1 - dot * q0);

        return cos(theta) * q0 + sin(theta) * q1;
    }
#pragma endregion

#pragma region Factory Methods
    Quaternion Quaternion::AxisAngle(const Vector3& axis, const float& angle)
    {
        float theta = Deg2Rad * angle;
        float sint = sin(theta / 2);
        float cost = cos(theta / 2);
        Vector3 v = sint * Vector3::Normalize(axis);
        return Quaternion(
            v.x,
            v.y,
            v.z,
            cost
        );
    }
    
    Quaternion Quaternion::Euler(const float& x, const float& y, const float& z)
    {
        float yaw = Deg2Rad * x / 2;
        float pitch = Deg2Rad * y / 2;
        float roll = Deg2Rad * z / 2;

        float cy = cos(yaw);
        float cp = cos(pitch);
        float cr = cos(roll);
        float sy = sin(yaw);
        float sp = sin(pitch);
        float sr = sin(roll);

        return Quaternion(
            cy * cp * sr - sy * sp * cr,
            sy * cp * sr + cy * sp * cr,
            sy * cp * cr - cy * sp * sr,
            cy * cp * cr + sy * sp * sr
        );
    }

    Quaternion Quaternion::Euler(const Vector3& angles)
    {
        return Quaternion::Euler(angles.x, angles.y, angles.z);
    }
#pragma endregion


#pragma region Operators
    bool operator==(const Quaternion& a, const Quaternion& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
    bool operator!=(const Quaternion& a, const Quaternion& b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

    Quaternion operator+(const Quaternion& a, const Quaternion& b) { return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    Quaternion operator-(const Quaternion& a, const Quaternion& b) { return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    Quaternion operator-(const Quaternion& a) { return Quaternion(-a.x, -a.y, -a.z, -a.w); }

    Quaternion operator*(const float& a, const Quaternion& b) { return Quaternion(a * b.x, a * b.y, a * b.z, a * b.w); }
    Quaternion operator*(const Quaternion& a, const float& b) { return Quaternion(a.x * b, a.y * b, a.z * b, a.w * b); }

    Quaternion operator*(const Quaternion& a, const Quaternion& b)
    {
        return Quaternion(
            a.x*b.w + a.w*b.x - a.z*b.y + a.y*b.z,
            a.y*b.w + a.z*b.x + a.w*b.y - a.x*b.z,
            a.z*b.w - a.y*b.x + a.x*b.y + a.w*b.z,
            a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
        );
    }

    Quaternion& operator*=(Quaternion& a, const Quaternion& b)
    {
        a = a * b;
        return a;
    }

    std::ostream& operator<<(std::ostream& out, const Quaternion& q) { return out << "Quaternion(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")"; }
#pragma endregion

}