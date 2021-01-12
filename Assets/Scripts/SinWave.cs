using NoEngine;
using System;

namespace Scripts
{
    public class SinWave : Script
    {
        public float magnitude = 1.0f;
        public float speed = 1.0f;

        void Update()
        {
            transform.position.y += magnitude * speed * Time.deltaTime;
            transform.SetPosition(new Vector3(0, 1, 0));
        }
    }
}
