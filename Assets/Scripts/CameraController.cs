using NoEngine;
using System;

namespace Scripts
{
    public class CameraController : Script
    {
        // Exposed Property
        public float movementSpeed = 5.0f;
        public float rotateSpeed = 90.0f;

        private bool _enabled = false;
        private Vector3 _movementInput = Vector3.Zero;
        private Vector2 _lastMousePos = new Vector2(0, 0);

        void Update()
        {
            if (_enabled)
            {
                // Update Position
                var right = transform.Right;
                var up = Vector3.Up;
                var forward = transform.Forward;

                var movement = _movementInput.x * right + _movementInput.y * up + _movementInput.z * forward;
                movement = Time.deltaTime * movementSpeed * movement;
                transform.Position += movement;

                // Update Rotation
                var delta = Time.deltaTime * rotateSpeed;
                var drag = Input.mousePos - _lastMousePos;

                transform.Rotation = Quaternion.Euler(0, drag.x * delta, 0) * Quaternion.AxisAngle(right, drag.y * delta) * transform.Rotation;
            }
            _lastMousePos = Input.mousePos;
        }

        void OnInput(InputEvent e)
        {
            if (e.phase == InputEventPhase.PRESS && e.keyCode == 67 /* C */)
            {
                _enabled = !_enabled;
                if (_enabled == false)
                {
                    _movementInput = Vector3.Zero;
                }
            }

            if (_enabled)
            {
                if (e.phase == InputEventPhase.PRESS || e.phase == InputEventPhase.RELEASE)
                {
                    float state = e.phase == InputEventPhase.PRESS ? 1.0f : -1.0f;
                    if (e.keyCode == 68 /* D */)
                    {
                        _movementInput.x += state;
                    }
                    if (e.keyCode == 65 /* A */)
                    {
                        _movementInput.x -= state;
                    }
                    if (e.keyCode == 32 /* SPACE */)
                    {
                        _movementInput.y += state;
                    }
                    if (e.keyCode == 340 /* LEFT_SHIFT */)
                    {
                        _movementInput.y -= state;
                    }
                    if (e.keyCode == 87 /* W */)
                    {
                        _movementInput.z += state;
                    }
                    if (e.keyCode == 83 /* S */)
                    {
                        _movementInput.z -= state;
                    }
                }
            }
        }
    }
}