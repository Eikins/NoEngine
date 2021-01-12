using System.Runtime.CompilerServices;

namespace NoEngine
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }

    public class Transform
    {
        public Vector3 position;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void SetPosition(Vector3 position);
    }
}