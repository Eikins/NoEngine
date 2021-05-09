using System.Runtime.CompilerServices;
using System;

namespace NoEngine
{
    public struct Transform
    {
        private IntPtr _nativeHandle;

        public Vector3 Position
        {
            get { return GetPosition(_nativeHandle); }
            set { SetPosition(_nativeHandle, value); }
        }

        public Quaternion Rotation
        {
            get { return GetRotation(_nativeHandle); }
            set { SetRotation(_nativeHandle, value); }
        }

        public Vector3 Scale
        {
            get { return GetScale(_nativeHandle); }
            set { SetScale(_nativeHandle, value); }
        }

        public Vector3 Right { get { return GetRightVector(_nativeHandle); } }
        public Vector3 Up { get { return GetUpVector(_nativeHandle); } }
        public Vector3 Forward { get { return GetForwardVector(_nativeHandle); } }

        #region Internal Calls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void SetPosition(IntPtr handle, Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void SetRotation(IntPtr handle, Quaternion rotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void SetScale(IntPtr handle, Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetPosition(IntPtr handle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Quaternion GetRotation(IntPtr handle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetScale(IntPtr handle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetRightVector(IntPtr handle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetUpVector(IntPtr handle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Vector3 GetForwardVector(IntPtr handle);
        #endregion
    }
}