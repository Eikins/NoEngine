using System;

namespace NoEngine
{
    public struct Vector3 : IEquatable<Vector3>
    {
        public static readonly Vector3 Zero = new Vector3(0, 0, 0);
        public static readonly Vector3 Right = new Vector3(1, 0, 0);
        public static readonly Vector3 Up = new Vector3(0, 1, 0);
        public static readonly Vector3 Forward = new Vector3(0, 0, 1);
        public static readonly Vector3 One = new Vector3(1, 1, 1);

        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 Normalize(Vector3 vector)
        {
            if (vector == Vector3.Zero) return vector;

            float magnitude = (float) Math.Sqrt((double) (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
            return new Vector3(vector.x / magnitude, vector.y / magnitude, vector.z / magnitude);
        }

        public bool Equals(Vector3 other) { return this == other; }
        public override bool Equals(object other)
        {
            if (!(other is Vector3)) return false;
            return this.Equals((Vector3) other);
        }

        public override int GetHashCode()
        {
            return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode();
        }

        #region Operators
        public static bool operator ==(Vector3 a, Vector3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
        public static bool operator !=(Vector3 a, Vector3 b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
        public static Vector3 operator +(Vector3 a, Vector3 b) { return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static Vector3 operator -(Vector3 a, Vector3 b) { return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static Vector3 operator *(float a, Vector3 b) { return new Vector3(a * b.x, a * b.y, a * b.z); }
        #endregion
    }
}