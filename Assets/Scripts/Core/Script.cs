using System;

namespace NoEngine
{
    public class Script
    {
        private Transform _transform;

        protected Transform transform
        {
            get { return _transform; }
        }

        protected T GetComponent<T>()
        {
            Console.WriteLine("C# name : " + typeof(T).Name);
            return null;
        }
    }
}