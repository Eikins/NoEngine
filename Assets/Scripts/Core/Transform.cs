using System.Runtime.CompilerServices;
using System;

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

    public struct Transform
    {
        private IntPtr _nativeHandle;

        public void SetPosition(Vector3 position)
        {
            SetPosition(_nativeHandle, position);
        }

        public Vector3 GetPosition()
        {
            return GetPosition(_nativeHandle);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void SetPosition(IntPtr handle, Vector3 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetPosition(IntPtr handle);
    }
}