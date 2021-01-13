using NoEngine;

namespace Scripts
{
    public class SinWave : Script
    {
        // Exposed Fields
        public float magnitude = 1.0f;
        public float speed = 1.0f;

        // Cache
        private Renderer _renderer;
        private Material _material;

        // Called before the first Update
        void Init()
        {
            _renderer = GetComponent<Renderer>();
            _material = _renderer.GetMaterial();
        }

        // Called each frame
        void Update()
        {
            float sint = (float)Math.Sin((double)speed * Time.time);

            // Update Position
            var position = transform.GetPosition();
            position.x = magnitude * sint;
            transform.SetPosition(position);

            // Update material properties
            _material.SetColor("Color", new Color(0.2f, 0.2f, 0.5f + 0.3f * sint, 0f));
        }
    }
}
