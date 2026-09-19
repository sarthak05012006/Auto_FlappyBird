# Auto_FlappyBird
🤖 Project concept: Auto Flappy Bird  The microcontroller watches the game screen, determines the bird's vertical position and the next pipe's position, then automatically generates a flap input.




Basic architecture:
        FLAPPY BIRD GAME
              │
              ▼
       ┌───────────────┐
       │ Camera/Sensor │
       │ Detect screen │
       └───────┬───────┘
               │
               ▼
       ┌───────────────┐
       │ ESP32 / Arduino│
       │ Decision Logic │
       └───────┬───────┘
               │
          FLAP SIGNAL
               │
               ▼
       ┌───────────────┐
       │ Servo / Relay │
       │ / HID Control │
       └───────┬───────┘
               │
               ▼
          SPACE / TAP
               │
               ▼
       🐦 FLAPPY BIRD


🔧 Hardware I'd used : 
  | Component                  | Purpose                           |
| -------------------------- | --------------------------------- |
| **ESP32**                  | Main controller                   |
| **Camera module / OV2640** | See the game screen               |
| **Servo motor**            | Physically press/tap the keyboard |
| **OLED display**           | Show score/status                 |
| Push button                | Start/stop automatic mode         |
| Buzzer                     | Status/error indication           |
 ----------------------------------------------------------------


🧠 How the automatic AI works:
  The ESP32 repeatedly:
    1.Captures the game screen.
    2.Detects the bird.
    3.Detects the upper and lower pipe boundaries.
    4.Calculates the safe gap.
    5.Estimates whether the bird is moving upward/downward.
    6.Predicts where the bird will be when it reaches the pipe.
    7.If the bird is too low or predicted to hit the pipe → FLAP.
    8.Otherwise → do nothing.
    9.Repeat ~10–30 times per second.
    


