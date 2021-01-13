using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;

namespace NoEngine
{
    public class Script
    {
        static readonly Dictionary<Type, int> ComponentNativeType = new Dictionary<Type, int>()
        {
            { typeof(Renderer), 1 }
        };

        private Transform _transform = default;

        protected ref Transform transform
        {
            get { return ref _transform; }
        }

        protected T GetComponent<T>() where T : Component
        {
            var handle = GetComponentHandle(_transform, ComponentNativeType[typeof(T)]);
            return handle.ToInt32() == 0 ? null : Activator.CreateInstance(typeof(T), handle) as T;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static IntPtr GetComponentHandle(Transform transform, int type);
    }
}