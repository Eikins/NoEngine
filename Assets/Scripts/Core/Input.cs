using System;

namespace NoEngine
{
    public enum InputEventPhase : Int32
    {
        PRESS = 0,
        REPEAT = 1,
        RELEASE = 2
    }

    public struct InputEvent
    {
        public InputEventPhase phase;
        public Int32 keyCode;
    }

    public class Input
    {
        public static Vector2 mousePos;
    }
}