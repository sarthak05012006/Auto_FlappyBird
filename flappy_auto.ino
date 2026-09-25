#include "esp_camera.h"
#include <ESP32Servo.h>

// ================= CAMERA PINS =================
// AI-Thinker ESP32-CAM
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

// ================= SERVO =================
#define SERVO_PIN 13

Servo keyServo;

// Servo positions
#define SERVO_RELEASE  70
#define SERVO_PRESS    115

// ================= GAME SETTINGS =================

// Approximate location of bird in camera frame
#define BIRD_X 45

// Vertical position considered dangerous
#define SAFE_TOP     35
#define SAFE_BOTTOM  100

// How often camera is checked
#define DETECTION_DELAY 80

// Minimum time between key presses
#define FLAP_COOLDOWN 250

unsigned long lastFlap = 0;


// =================================================
// CAMERA SETUP
// =================================================

void setupCamera() {

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

  config.pixel_format = PIXFORMAT_GRAYSCALE;

  // Small image = faster processing
  config.frame_size = FRAMESIZE_QQVGA;

  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.print("Camera initialization failed: 0x");
    Serial.println(err, HEX);

    while (true) {
      delay(1000);
    }
  }

  Serial.println("Camera initialized!");
}


// =================================================
// SERVO FLAP
// =================================================

void flap() {

  unsigned long now = millis();

  if (now - lastFlap < FLAP_COOLDOWN) {
    return;
  }

  lastFlap = now;

  Serial.println("FLAP!");

  // Press keyboard key
  keyServo.write(SERVO_PRESS);

  delay(100);

  // Release key
  keyServo.write(SERVO_RELEASE);

  delay(50);
}


// =================================================
// SIMPLE IMAGE ANALYSIS
// =================================================
//
// This is a basic vision algorithm.
// It looks for darker pixels around the
// expected bird location.
//
// It is intended as a starting point and
// requires calibration for your particular
// Flappy Bird screen.
//

int detectBirdY(camera_fb_t *fb) {

  int bestY = -1;
  int bestScore = 0;

  // Search area around bird
  int startX = 20;
  int endX = 70;

  int startY = 10;
  int endY = 115;

  for (int y = startY; y < endY; y += 3) {

    int score = 0;

    for (int x = startX; x < endX; x += 3) {

      int index = y * fb->width + x;

      if (index >= fb->len) {
        continue;
      }

      uint8_t pixel = fb->buf[index];

      // Detect dark object
      if (pixel < 80) {
        score++;
      }
    }

    if (score > bestScore) {
      bestScore = score;
      bestY = y;
    }
  }

  return bestY;
}


// =================================================
// SIMPLE PIPE DETECTION
// =================================================

int detectObstacleY(camera_fb_t *fb) {

  int obstacleY = -1;

  int strongest = 0;

  // Look in front of bird
  int startX = 80;
  int endX = 140;

  for (int y = 10; y < 115; y += 3) {

    int score = 0;

    for (int x = startX; x < endX; x += 3) {

      int index = y * fb->width + x;

      if (index >= fb->len) {
        continue;
      }

      uint8_t pixel = fb->buf[index];

      // Dark obstacle
      if (pixel < 80) {
        score++;
      }
    }

    if (score > strongest) {
      strongest = score;
      obstacleY = y;
    }
  }

  return obstacleY;
}


// =================================================
// SETUP
// =================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("AUTOMATIC FLAPPY BIRD PLAYER");
  Serial.println("==============================");

  // Servo
  keyServo.setPeriodHertz(50);

  keyServo.attach(
    SERVO_PIN,
    500,
    2400
  );

  // Start in released position
  keyServo.write(SERVO_RELEASE);

  delay(500);

  // Camera
  setupCamera();

  Serial.println("System ready!");
  Serial.println("Starting in 3 seconds...");

  delay(3000);

  Serial.println("GAME START!");
}


// =================================================
// MAIN LOOP
// =================================================

void loop() {

  // Capture image
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {

    Serial.println("Camera capture failed");

    delay(100);

    return;
  }


  // Detect bird
  int birdY = detectBirdY(fb);

  // Detect obstacle
  int obstacleY = detectObstacleY(fb);


  // Return frame buffer
  esp_camera_fb_return(fb);


  // Debug information
  Serial.print("Bird Y = ");
  Serial.print(birdY);

  Serial.print(" | Obstacle Y = ");
  Serial.println(obstacleY);


  // =============================================
  // FLIGHT CONTROL
  // =============================================

  if (birdY != -1) {

    // Bird too low
    if (birdY > SAFE_BOTTOM) {

      Serial.println("Bird LOW -> FLAP");

      flap();
    }

    // Bird too high
    else if (birdY < SAFE_TOP) {

      Serial.println("Bird HIGH -> NO FLAP");
    }

    // Bird in middle
    else {

      // If obstacle detected near bird,
      // flap to move upward
      if (obstacleY != -1) {

        if (obstacleY > birdY + 15) {

          Serial.println("Obstacle LOW -> FLAP");

          flap();
        }
      }
    }
  }


  delay(DETECTION_DELAY);
}
