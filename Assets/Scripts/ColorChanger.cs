using System;
using NoEngine;

namespace Scripts
{
    public class ColorChanger : Script
    {
        // Exposed Fields
        public float speed = 30.0f;
        public float saturation = 0.5f;
        public float value = 0.5f;

        // Cache
        private Renderer _renderer;
        private Material _material;
        private float _hueAngle;

        // Called before the first Update
        void Init()
        {
            _renderer = GetComponent<Renderer>();
            _material = _renderer.GetMaterial();
            _hueAngle = 0f;
        }

        // Called each frame
        void Update()
        {
            _hueAngle += speed * Time.deltaTime;
            _hueAngle = _hueAngle % 360f;
            var color = HsvToRgb(new Vector3(_hueAngle, saturation, value));

            // Update material properties
            _material.SetColor("Color", new Color(color.x, color.y, color.z, 1.0f));
        }

        #region Color Operation
        // http://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c/

        /// <summary>
        /// Convert HSV to RGB
        /// h is from 0-360
        /// s,v values are 0-1
        /// r,g,b values are 0-255
        /// Based upon http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space#HSV_Transformation_C_.2F_C.2B.2B_Code_2
        /// </summary>
        private Vector3 HsvToRgb(Vector3 hsv)
        {
            // ######################################################################
            // T. Nathan Mundhenk
            // mundhenk@usc.edu
            // C/C++ Macro HSV to RGB

            float H = hsv.x;
            H = H % 360f;

            if (hsv.z <= 0f)
            {
                return Vector3.Zero;
            }
            else if (hsv.y <= 0f)
            {
                return new Vector3(hsv.z, hsv.z, hsv.z);
            }
            else
            {
                float hf = H / 60.0f;
                int i = (int)Math.Floor((double)hf);
                float f = hf - i;
                float pv = hsv.z * (1 - hsv.y);
                float qv = hsv.z * (1 - hsv.y * f);
                float tv = hsv.z * (1 - hsv.y * (1 - f));

                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;
                switch (i)
                {
                    case 0:
                        r = hsv.z;
                        g = tv;
                        b = pv;
                        break;
                    case 1:
                        r = qv;
                        g = hsv.z;
                        b = pv;
                        break;
                    case 2:
                        r = pv;
                        g = hsv.z;
                        b = tv;
                        break;
                    case 3:
                        r = pv;
                        g = qv;
                        b = hsv.z;
                        break;
                    case 4:
                        r = tv;
                        g = pv;
                        b = hsv.z;
                        break;
                    default:
                        r = hsv.z;
                        g = pv;
                        b = qv;
                        break;
                }
                return new Vector3(r, g, b);
            }
        }
        #endregion
    }
}
