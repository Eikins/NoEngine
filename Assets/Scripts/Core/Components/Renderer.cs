using System.Runtime.CompilerServices;
using System;

namespace NoEngine
{
    public class Renderer : Component
    {
        public Renderer(IntPtr handle) : base(handle) { }

        public Material GetMaterial()
        {
            return GetMaterial(GetNativeHandle());
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static Material GetMaterial(IntPtr rendererHandle);
    }
}