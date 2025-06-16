# RTSS v0.7 - Real-Time Speed of Sound Monitor

Platform: ESP32 | HC-SR04 | DHT11  
Version: 0.7  
June 2025

---

## Overview

RTSS (Real-Time Speed of Sound) is an ESP32-based project that monitors the speed of sound through air by:

- Measuring echo time via HC-SR04 (ultrasonic sensor) using interrupt-based timing, and calculating speed = distance/time;
- Reading temperature and humidity via DHT11;
- Comparing **experimental vs theoretical** speeds of sound.

All results are logged over Serial Monitor.

---

## Core Concepts

- Speed of sound depends on **temperature and humidity**.
- Echo time is measured using **rising and falling edge interrupts**.
- Geometry correction applied to account for **sensor offset**.

### Theoretical speed formula:

v = 331.3 + 0.606T + 0.0124H

where, T is the the temperature in degree C, and H is %age relative humidity.

---

## 🔩 Hardware Setup

|Components|  GPIO Pin | Function         |
|----------|-----------|------------------|
| HC-SR04  | GPIO 27,25| Trigger, Echo    |
| DHT11    | GPIO 23   | Temp & Hum       |
| ESP32    | USB       | Power + Serial   |

### Physical Setup:
- Distance from sensor to reflector = **25.6 cm** (vertical).
- Correction applied:  
  L<sub> corrected</sub> = $\sqrt{25.6^2+1.05^2}$ -- approx 25.62 cm

See `/media` folder for reference images.

---

## How to Use

1. Upload the `RTSS_v0.7.ino` to your ESP32.
2. Make an equivalent setup according to the image attached
3. Open Serial Monitor at `115200 baud`.
4. Type **`x`** to trigger 10 readings.
