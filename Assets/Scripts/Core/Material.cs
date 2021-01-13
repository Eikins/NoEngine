using System.Runtime.CompilerServices;
using System;

namespace NoEngine
{
    public struct Color
    {
        float r, g, b, a;

        public Color(float r, float g, float b, float a)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }
    }

    public struct Material
    {
        private IntPtr _nativeHandle;

        public void SetColor(string propertyName, Color color)
        {
            SetColor(_nativeHandle, propertyName, color);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void SetColor(IntPtr handle, string propertyName, Color color);
    }
}