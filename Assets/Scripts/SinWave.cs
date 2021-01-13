using System;
using NoEngine;

namespace Scripts
{
    public class SinWave : Script
    {
        // Exposed Fields
        public float magnitude = 1.0f;
        public float speed = 1.0f;

        // Called each frame
        void Update()
        {
            float sint = (float) Math.Sin((double) speed * Time.time);

            // Update Position
            var position = transform.Position;
            position.x = magnitude * sint;
            transform.Position = position;
        }
    }
}
