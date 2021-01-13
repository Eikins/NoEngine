using System;

namespace NoEngine
{
    public struct Quaternion
    {
        public static readonly Quaternion Identity = new Quaternion(0, 0, 0, 1);

        private float x, y, z, w;

        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static Quaternion operator * (Quaternion a, Quaternion b)
        {
            return new Quaternion(
                a.x* b.w + a.w* b.x - a.z* b.y + a.y* b.z,
                a.y* b.w + a.z* b.x + a.w* b.y - a.x* b.z,
                a.z* b.w - a.y* b.x + a.x* b.y + a.w* b.z,
                a.w* b.w - a.x* b.x - a.y* b.y - a.z* b.z
            );
        }

        public static Quaternion AxisAngle(Vector3 axis, float angle)
        {
            float theta = MathConstants.Deg2Rad * angle;
            float sint = (float) Math.Sin((double) theta / 2);
            float cost = (float) Math.Cos((double) theta / 2);
            var v = sint * Vector3.Normalize(axis);
            return new Quaternion(
                v.x,
                v.y,
                v.z,
                cost
            );
        }

        public static Quaternion Euler(float x, float y, float z)
        {
            float yaw = MathConstants.Deg2Rad * x / 2;
            float pitch = MathConstants.Deg2Rad * y / 2;
            float roll = MathConstants.Deg2Rad * z / 2;

            float cy = (float) Math.Cos((double) yaw);
            float cp = (float) Math.Cos((double) pitch);
            float cr = (float) Math.Cos((double) roll);
            float sy = (float) Math.Sin((double) yaw);
            float sp = (float) Math.Sin((double) pitch);
            float sr = (float) Math.Sin((double) roll);

            return new Quaternion(
                cr* cp * sy - sr* sp * cy,
                sr* cp * sy + cr* sp * cy,
                sr* cp * cy - cr* sp * sy,
                cr* cp * cy + sr* sp * sy
            );
        }
}
}