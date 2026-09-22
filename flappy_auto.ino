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


// ======================================================
// Detect bird
// Yellow/orange bird
// ======================================================

bool isBird(int r, int g, int b)
{
  return (
    r > 120 &&
    g > 80 &&
    b < 100 &&
    r > b * 1.4
  );
}


// ======================================================
// Detect bird position
// ======================================================

void detectBird(camera_fb_t *fb)
{
  long sumX = 0;
  long sumY = 0;
  int count = 0;

  uint16_t *buffer = (uint16_t *)fb->buf;

  // Ignore edges
  for (int y = 20; y < FRAME_HEIGHT - 20; y += 2)
  {
    for (int x = 10; x < FRAME_WIDTH - 10; x += 2)
    {
      uint16_t pixel = buffer[y * FRAME_WIDTH + x];

      int r, g, b;
      rgb565ToRGB(pixel, r, g, b);

      if (isBird(r, g, b))
      {
        sumX += x;
        sumY += y;
        count++;
      }
    }
  }

  if (count > 5)
  {
    birdX = sumX / count;
    birdY = sumY / count;
  }
  else
  {
    birdX = -1;
    birdY = -1;
  }
}


// ======================================================
// Detect nearest green pipe
// ======================================================

void detectPipe(camera_fb_t *fb)
{
  uint16_t *buffer = (uint16_t *)fb->buf;

  pipeX = -1;
  gapTop = -1;
  gapBottom = -1;

  // Start searching just ahead of bird
  int searchStart = 80;

  if (birdX > 0)
    searchStart = birdX + 30;

  if (searchStart >= FRAME_WIDTH)
    return;

  // Search for first strong vertical green pipe
  for (int x = searchStart; x < FRAME_WIDTH - 10; x += 3)
  {
    int greenCount = 0;

    for (int y = 0; y < FRAME_HEIGHT; y += 3)
    {
      uint16_t pixel = buffer[y * FRAME_WIDTH + x];

      int r, g, b;
      rgb565ToRGB(pixel, r, g, b);

      if (isPipe(r, g, b))
        greenCount++;
    }

    // Enough green pixels = pipe
    if (greenCount > 15)
    {
      pipeX = x;
      break;
    }
  }

  if (pipeX == -1)
    return;


  // Find the gap
  bool insideGap = false;

  for (int y = 0; y < FRAME_HEIGHT; y += 2)
  {
    uint16_t pixel = buffer[y * FRAME_WIDTH + pipeX];

    int r, g, b;
    rgb565ToRGB(pixel, r, g, b);

    bool green = isPipe(r, g, b);

    if (!green && !insideGap)
    {
      gapTop = y;
      insideGap = true;
    }

    if (green && insideGap)
    {
      gapBottom = y;
      break;
    }
  }

  // If gap continues to bottom
  if (insideGap && gapBottom == -1)
    gapBottom = FRAME_HEIGHT - 1;
}


// ======================================================
// Flap servo
// ======================================================

void flap()
{
  unsigned long now = millis();

  if (now - lastFlap < FLAP_DELAY)
    return;

  Serial.println(">>> FLAP!");

  flapServo.write(SERVO_PRESS);

  delay(70);

  flapServo.write(SERVO_IDLE);

  lastFlap = millis();
}


// ======================================================
// Game decision
// ======================================================

void makeDecision()
{
  if (birdY < 0)
  {
    Serial.println("Bird not detected");
    return;
  }

  if (gapTop < 0 || gapBottom < 0)
  {
    Serial.println("Pipe not detected");

    // Safety: if bird gets too low
    if (birdY > 175)
      flap();

    return;
  }

  int gapCenter = (gapTop + gapBottom) / 2;

  Serial.print("Bird Y: ");
  Serial.print(birdY);

  Serial.print(" | Pipe X: ");
  Serial.print(pipeX);

  Serial.print(" | Gap Center: ");
  Serial.println(gapCenter);


  // ==================================================
  // Automatic control
  // ==================================================

  int error = birdY - gapCenter;


  // Bird is below safe center
  if (error > 20)
  {
    flap();
  }

  // Bird is very low
  if (birdY > 195)
  {
    flap();
  }
}


// ======================================================
// Camera setup
// ======================================================

void setupCamera()
{
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;

  config.pixel_format = PIXFORMAT_RGB565;

  // Low resolution for faster processing
  config.frame_size = FRAMESIZE_QVGA;

  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK)
  {
    Serial.print("Camera initialization failed: ");
    Serial.println(err);
    while (true);
  }

  Serial.println("Camera initialized!");
}


// ======================================================
// SETUP
// ======================================================

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("==============================");
  Serial.println("AUTO FLAPPY BIRD");
  Serial.println("==============================");

  // Servo
  flapServo.setPeriodHertz(50);
  flapServo.attach(SERVO_PIN, 500, 2400);

  flapServo.write(SERVO_IDLE);

  // Camera
  setupCamera();

  Serial.println("System Ready!");
}


// ======================================================
// LOOP
// ======================================================

void loop()
{
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }

  // Detect bird
  detectBird(fb);

  // Detect pipe and safe gap
  detectPipe(fb);

  // Make automatic decision
  makeDecision();

  // Return frame buffer
  esp_camera_fb_return(fb);

  // Small delay
  delay(30);
}
