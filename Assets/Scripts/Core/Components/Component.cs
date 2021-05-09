using System;

namespace NoEngine
{
    public class Component
    {
        private IntPtr _nativeHandle;

        public Component(IntPtr nativeHandle)
        {
            _nativeHandle = nativeHandle;
        }

        protected IntPtr GetNativeHandle()
        {
            return _nativeHandle;
        }
    }
}