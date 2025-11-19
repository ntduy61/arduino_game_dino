# Dino 404 Arduino (SH1106 OLED Version)

This project recreates a minimal Chrome Dino-style game running on an Arduino using an SH1106 OLED screen. The Dino character is represented as a rolling 16x16 circle with a simple rotation animation. Obstacles are triangle-shaped cactuses, and the ground is drawn with tiled squares. The game includes jump mechanics, collision detection, scoring, and a restart screen.

## Features

* Smooth rolling animation for the Dino
* Small jump for a responsive gameplay feel
* Triangle cactus obstacle
* Ground rendered using small 4x4 tiles
* Score counter
* Game Over + restart logic

## Hardware Requirements

* Arduino (UNO/Nano/Pro Mini)
* SH1106 128x64 OLED display (I2C)
* Push button (jump control)
* Optional: resistor for button

## Wiring

**OLED SH1106**

* VCC → 5V
* GND → GND
* SCK → A5
* SDA → A4
![20251120_001402](https://github.com/user-attachments/assets/a730edb4-e338-4ff7-8e92-2f9ec5ae6e96)

**Button**

* One side → Pin 2
* Other side → GND

Using internal pull-up: `pinMode(BTN, INPUT_PULLUP);`
![20251120_001402](https://github.com/user-attachments/assets/24f563e2-caac-4b46-a41b-5924be8b88a0)

## Libraries Used

* Adafruit_GFX
* Adafruit_SH110X

Make sure to install these libraries via the Arduino Library Manager.

## How to Play

* Press the button to make the Dino jump.
* Avoid hitting the cactus.
* Each time the obstacle resets, your score increases.
* When you lose, press the button again to restart.

## Code

The main game logic includes:

* Player physics (gravity + jump)
* Rolling animation frames
* Obstacle movement
* Collision detection
* Drawing the ground, Dino, cactus, and score

## License

Free to use, modify, and experiment with in your Arduino projects.
