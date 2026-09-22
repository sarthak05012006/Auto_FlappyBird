#include "esp_camera.h"
#include <ESP32Servo.h>

// ===============================
// AI Thinker ESP32-CAM pins
// ===============================

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34 
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ===============================
// Servo
// ===============================

#define SERVO_PIN 13

Servo flapServo;

// Servo angles
#define SERVO_IDLE   0
#define SERVO_PRESS  55

// ===============================
// Camera resolution
// ===============================

#define FRAME_WIDTH  320
#define FRAME_HEIGHT 240

// ===============================
// Bird detection
// ===============================

int birdX = -1;
int birdY = -1;

// ===============================
// Pipe detection
// ===============================

int pipeX = -1;
int gapTop = -1;
int gapBottom = -1;

// ===============================
// Timing
// ===============================

unsigned long lastFlap = 0;

// Minimum time between flaps
const unsigned long FLAP_DELAY = 180;

// ======================================================
// RGB565 conversion
// ======================================================

void rgb565ToRGB(uint16_t pixel, int &r, int &g, int &b)
{
  r = ((pixel >> 11) & 0x1F) << 3;
  g = ((pixel >> 5) & 0x3F) << 2;
  b = (pixel & 0x1F) << 3;
}


// ======================================================
// Detect green pipe
// ======================================================

bool isPipe(int r, int g, int b)
{
  return (
    g > 90 &&
    g > r * 1.25 &&
    g > b * 1.15
  );
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
