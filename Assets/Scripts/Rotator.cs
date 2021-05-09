using System;
using NoEngine;

namespace Scripts
{
    public class Rotator : Script
    {
        // Exposed Fields
        public float angularSpeed = 30.0f;

        // Called each frame
        void Update()
        {
            // Rotate around Y Axis
            transform.Rotation *= Quaternion.AxisAngle(Vector3.Up, Time.deltaTime * angularSpeed);
        }
    }
}
