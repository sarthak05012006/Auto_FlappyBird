# 🐦 Auto Flappy Bird — Hardware-Based Autonomous Game

An autonomous hardware implementation of the classic **Flappy Bird** game where the system detects obstacles and automatically controls the bird's movement using sensors and a servo mechanism.

The project demonstrates the combination of **embedded systems, sensors, automation, and real-time decision making** to create a physical version of Flappy Bird.

---

## 🚀 Project Overview

Traditional Flappy Bird requires a player to continuously control the bird by clicking or tapping.

**Auto Flappy Bird** removes the need for manual control.

The system continuously monitors the game environment, detects upcoming obstacles, and automatically generates the required control action to keep the bird flying through the gap.

### Core Concept

**Detect → Analyse → Decide → Act**

```text
        ┌─────────────────┐
        │  Game Display   │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │ Sensor / Camera  │
        │    Detection     │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │      ESP32       │
        │ Decision Making  │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │ Servo / Control  │
        │    Mechanism     │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │   Bird Control  │
        └─────────────────┘
```

---

## ✨ Features

* 🤖 Fully autonomous gameplay
* 👁️ Real-time obstacle detection
* 🧠 Automatic decision making
* ⚙️ Servo-based physical control
* 🔌 ESP32-based embedded system
* ⚡ Fast response to obstacles
* 🎮 No continuous human input required
* 📊 Can be extended with AI/computer vision
* 🔧 Suitable for robotics and IoT demonstrations

---

## 🛠️ Hardware Components

| Component        | Purpose                              |
| ---------------- | ------------------------------------ |
| ESP32            | Main controller                      |
| Camera / Sensor  | Detects bird and obstacles           |
| Servo Motor      | Produces the physical control action |
| Flappy Bird Game | Environment being controlled         |
| Jumper Wires     | Connections                          |
| Breadboard       | Prototyping                          |
| USB Cable        | Programming and power                |

### Optional Components

* OLED Display
* LDR sensor array
* Buzzer
* External power supply
* Additional sensors

---

## 💻 Software & Technologies

* **Arduino IDE**
* **C/C++**
* **ESP32**
* **ESP32 Camera Library**
* **ESP32Servo Library**
* Computer Vision / Sensor-based detection
* Real-time embedded control

---

# 🔄 Working Principle

The system operates in a continuous feedback loop.

### 1. Environment Detection

The sensor/camera observes the Flappy Bird game area.

### 2. Obstacle Identification

The system identifies the upcoming pipe/obstacle and determines the available gap.

### 3. Position Analysis

The current position of the bird is compared with the detected safe region.

### 4. Decision Making

The ESP32 determines whether the bird needs an upward movement.

### 5. Physical Action

The servo motor activates the control mechanism.

### 6. Continuous Feedback

The system repeats the process continuously as new obstacles appear.

```text
START
  │
  ▼
Detect Game Environment
  │
  ▼
Detect Bird & Obstacles
  │
  ▼
Calculate Safe Gap
  │
  ▼
Is Bird in Safe Position?
  │
 ┌┴─────────────┐
 │              │
YES            NO
 │              │
 ▼              ▼
Continue      Activate
Monitoring     Servo
 │              │
 └──────┬───────┘
        ▼
   Repeat Loop
```

---

# 🔌 System Architecture

```text
              ┌─────────────────────┐
              │    Flappy Bird      │
              │   Game Environment  │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Camera / LDR Array  │
              │   Detection System  │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │        ESP32        │
              │ Processing & Logic  │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │    Servo Motor      │
              │ Control Mechanism   │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Automatic Bird Input │
              └─────────────────────┘
```

---

# 📷 Detection Methods

The project can be implemented using different detection approaches.

## Option 1 — Camera-Based Detection

An ESP32-CAM captures the game screen and processes the image to identify:

* Bird position
* Pipes
* Pipe gap
* Obstacle distance

### Advantages

* More flexible
* Can detect multiple visual elements
* Better suited for computer vision
* Can be upgraded to AI-based detection

---

## Option 2 — LDR-Based Detection

Multiple LDR sensors can be positioned around the game area.

The LDR array detects changes in light intensity corresponding to the bird and obstacles.

### Advantages

* Low cost
* Simple electronics
* Easy to prototype
* Does not require image processing

### Limitation

More sensors may be required for reliable position detection.

---

# ⚙️ Control Logic

The basic control algorithm is:

```text
IF obstacle is approaching
        ↓
Check bird position
        ↓
IF bird is below safe region
        ↓
Activate servo
        ↓
Bird moves upward
        ↓
Continue monitoring
```

The control system continuously adjusts the bird's movement according to the detected environment.

---

# 📁 Suggested Project Structure

```text
Auto-Flappy-Bird/
│
├── README.md
│
├── src/
│   └── auto_flappy_bird.ino
│
├── hardware/
│   ├── circuit_diagram.png
│   └── hardware_setup.png
│
├── images/
│   ├── project_setup.jpg
│   ├── working.jpg
│   └── architecture.png
│
├── docs/
│   └── project_report.pdf
│
└── LICENSE
```

---

# 🔧 Installation

## 1. Install Arduino IDE

Install the Arduino IDE on your computer.

## 2. Add ESP32 Board Support

Add the ESP32 board package to Arduino IDE.

## 3. Install Required Libraries

Depending on the implementation:

```text
ESP32Servo
esp_camera
```

For an LDR-based version, no computer-vision library is required.

## 4. Connect Hardware

Connect the ESP32, sensors/camera, and servo according to the circuit diagram.

## 5. Upload Code

Open:

```text
src/auto_flappy_bird.ino
```

Select the appropriate ESP32 board and COM/USB port.

Upload the program.

---

# 🎮 Demonstration

Once powered on:

```text
Power ON
   ↓
ESP32 Initializes
   ↓
Detection Starts
   ↓
Bird Position Detected
   ↓
Obstacle Detected
   ↓
Decision Made
   ↓
Servo Activated
   ↓
Bird Controlled Automatically
```

The system continues operating until the game ends or the system is powered off.

---

# 🌟 Innovation

The main innovation of this project is the conversion of a software-based game interaction into a **real-world autonomous embedded control system**.

Instead of a human continuously providing input, the system:

> **observes the environment → processes information → makes a decision → physically performs the action.**

This makes the project a simple demonstration of the same fundamental concept used in:

* Autonomous robots
* Computer vision systems
* Industrial automation
* Assistive technologies
* Autonomous vehicles
* Closed-loop control systems

---

# 🔮 Future Scope

The project can be further upgraded with:

### 🤖 AI-Based Detection

Use machine learning to detect the bird and pipes more accurately.

### 🧠 Predictive Control

Predict the bird's future position instead of reacting only to its current position.

### 📈 Reinforcement Learning

Train an AI agent to learn the best timing for each flap.

### 📷 Advanced Computer Vision

Use OpenCV or an ESP32-S3-based vision system for improved detection.

### 📊 Performance Dashboard

Track:

* Score
* Reaction time
* Successful obstacles
* Failure points
* Average control interval

### 🌐 IoT Integration

Send gameplay statistics to a web dashboard.

---

# 🎯 Applications

Although developed as a Flappy Bird project, the underlying concept demonstrates:

* Autonomous control
* Sensor fusion
* Embedded programming
* Real-time decision making
* Robotics
* Computer vision
* Human-machine interaction

---

# 👨‍💻 Project Team

**Project:** Auto Flappy Bird
**Domain:** Embedded Systems / Robotics / IoT
**Controller:** ESP32
**Control:** Servo Motor
**Detection:** Camera / LDR Sensor Array

---

# 📜 License

This project is intended for educational, experimental, and demonstration purposes.

---

## ⭐ Support

If you found this project interesting, consider giving the repository a ⭐ on GitHub!

**Built with ESP32 + Sensors + Automation + Creativity.** 🐦⚡
