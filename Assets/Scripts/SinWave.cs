using NoEngine;
using System;

using System.Runtime.InteropServices;

namespace Scripts
{
    public class SinWave : Script
    {
        public float magnitude = 1.0f;
        public float speed = 1.0f;

        void Update()
        {
            GetComponent<Renderer>();
            var position = transform.GetPosition();
            // Console.WriteLine("Allo");
            Console.WriteLine(position.x + ", " + position.y + ", " + position.z);
            float t = magnitude * (float)Math.Sin((double)speed * Time.time);
            transform.SetPosition(new Vector3(
                t,
                t,
                0));
        }
    }
}
