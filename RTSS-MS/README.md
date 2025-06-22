# RTSS-MS: Real-Time Speed of Sound Monitor

A complete yet lightweight setup combining embedded systems, IoT, and physics to measure the speed of sound in real time using ultrasonic sensing and environmental data, implemented on an ESP32 with ThingSpeak integration.

---

## 1. Objective

To compute and compare the speed of sound using:

* Measured time-of-flight (`S(D, Δt)`) from an ultrasonic sensor, D cm away from a parallelly aligned reflecting surface
* Theoretical estimation (`S(T, H)`) based on temperature and humidity using an empirical model
* Validate alignment via error analysis, and upload valid data to ThingSpeak

---

## 2. Theory

### S(D, Δt) — Distance-Time Based Speed of Sound

The speed of sound from distance and time-of-flight is calculated as:

$S(D, \Delta t) = \dfrac{D_{\text{corrected}}}{\Delta t / 2}$

Where:

* `D` is the distance from sensor to reflecting surface (in meters)
* `Δt` is the round-trip time (time of flight) measured using interrupts on the ESP32

Since the offset between the transmitter and receiver is 2.1cm, the effective distance is:

$D_{\text{corrected}} = \sqrt{D^2 + 1.05^2}$  

---

### S(T, H) — Wong–Embleton Empirical Model

An empirical correction to the dry-air sound speed formula is used for real-time embedded computation:

$S(T, H) = \left(331.3 + 0.606 \cdot T\right) \cdot \left[1 + \dfrac{H}{100} \cdot P(T)\right]$

Where:

* `T` = Temperature in °C
* `H` = Relative Humidity in %
* `P(T)` is a 4th-degree polynomial:

$P(T) = 9.66 \times 10^{-4} + 7.2 \times 10^{-5} \cdot T + 1.8 \times 10^{-6} \cdot T^2 + 7.2 \times 10^{-8} \cdot T^3 + 6.5 \times 10^{-11} \cdot T^4$

This model is accurate to \~0.1% in the range 0–30°C and 0–100% RH.

---

## 3. Hardware Setup

### Components used:

* ESP32 Dev Board (with built-in WiFi)
* HC-SR04 Ultrasonic Sensor
* DHT11 Temperature & Humidity Sensor
* Set Squares for alignment
* USB power, wires and breadboard setup

---

## 4. Implementation

### Interrupt-Based Time Capture

The echo pin is monitored using an interrupt on both rising and falling edges:

```cpp
void IRAM_ATTR timeRec() {
  if (digitalRead(echo) == HIGH) {
    start = esp_timer_get_time();
  } else {
    end = esp_timer_get_time();
    echo_received = true;
  }
}
```

Timestamps are captured using `esp_timer_get_time()` for microsecond accuracy.

### Data Processing Flow:

1. Measure `T` and `H` using DHT11
2. Trigger ultrasonic and measure time-of-flight
3. Compute `S_TH` and `S_D`
4. Check alignment (`|S_TH - S_D| ≤ threshold`)
5. Upload to ThingSpeak (if alignmentCheck returns true)

---

## 5. Error Analysis

### Primary Sources:

* **Echo Misalignment**: Oblique angles distort timing
* **Timing Errors**: ISR jitter or GPIO latency
* **Environmental Drift**: DHT11 has ±1°C / ±5% RH uncertainty
* **Distance Calibration**: Measurement error in `D` or height offset

### Mitigations:

* Added echo alignment check
* Used `noInterrupts()` and `volatile` guards for timing safety
* Empirical correction via Wong–Embleton model

---

## 6. Results

### Sample Data (June 22, 2025)

| Date & Time (UTC)   | Entry | Temp (°C) | Humidity (%) | S(T, H) (m/s) | S(D, Δt) (m/s) |
| ------------------- | ----- | --------- | ------------ | ------------- | -------------- |
| 2025-06-22 08:32:15 | 1     | 29.8      | 78           | 351.17548     | 351.94403      |
| 2025-06-22 08:33:32 | 2     | 29.9      | 78           | 351.24673     | 350.26007      |
| 2025-06-22 08:35:04 | 3     | 29.9      | 77           | 351.22330     | 350.26007      |
| 2025-06-22 08:38:50 | 4     | 29.9      | 78           | 351.24673     | 350.26007      |
| 2025-06-22 08:39:22 | 5     | 29.9      | 78           | 351.24673     | 350.97980      |
| 2025-06-22 08:39:53 | 6     | 29.9      | 79           | 351.27014     | 350.97980      |

> Observations: S(D, Δt) closely tracks S(T, H) with <1 m/s deviation.

---

## 7. Conclusion

RTSS-MS demonstrates real-time acoustic measurement using low-cost sensors and microcontroller-based computation. Wong–Embleton scaling enables embedded environmental correction with minimal overhead. Validated readings show sub-1% error across multiple cycles.

This system can be extended for:

* Environmental monitoring kits
* Real-time calibration experiments

---

## 8. Appendix

### Key Functions:

```cpp
float getS_D(uint64_t start, uint64_t end, float D_cm) {
  double time = 0.5 * (end - start) / 1e6;
  float D_m = sqrt(D_cm * D_cm + 1.05 * 1.05) / 100.0;
  return D_m / time;
}

float getS_TH(float T, float H) {
  float factor = (H / 100.0) * (
    9.66e-4 + 7.2e-5 * T + 1.8e-6 * T * T +
    7.2e-8 * T * T * T + 6.5e-11 * T * T * T * T
  );
  return (331.3 + 0.606 * T) * (1 + factor);
}
```
---

*Author: Parth Kadam (B.Tech ExTC, FE, SPIT)*
*Date: June 2025*
