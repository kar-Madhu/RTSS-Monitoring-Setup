## Speed of Sound in Humid Air — Empirical Model for RTSS-MS
---------------------------------------------------------

Overview
--------
The speed of sound in air depends on temperature, pressure, and humidity. 
For embedded and real-time applications, where simplicity and speed matter, 
an empirical model can provide accurate results over common environmental ranges 
without the computational cost of full acoustic calculations.

Base Model — Dry Air
---------------------
The ideal gas expression for the speed of sound in dry air is: 

$c = \sqrt{\frac{\gamma R T}{M}}$

Where:
- c: speed of sound [m/s]
- γ: adiabatic index for dry air ≈ 1.4
- R: universal gas constant ≈ 8.314 J/mol·K
- T: absolute temperature in Kelvin = (T_C + 273.15)
- M: molar mass of dry air ≈ 0.0289645 kg/mol

This can be approximated empirically for dry air at ambient pressure: 

$c_{\text{dry}}(T_C) \approx 331.3 + 0.606 \cdot T_C \quad (T_C \text{ in } ^\circ\mathrm{C})$

where T is in °C

Humidity Correction — Empirical Fit
-----------------------------------
Humidity increases the speed of sound because water vapor is lighter than dry air. 
Wong & Embleton (1980) provided an empirical correction factor as a 4th-degree 
polynomial fit P(T) over the range of 0°C to 30°C and relative humidity from 0% to 100%.

Their result expresses the speed of sound in humid air as:

 $ \frac{c_{\text{humid}}}{c_{\text{dry}}} = 1 + h \cdot P(T) $

Where:
- h = H / 100 : relative humidity as a fraction (0–1)
- T: temperature in °C
- P(T): polynomial in T

The polynomial is: 

$P(T) = 9.66 \times 10^{-4} + 7.2 \times 10^{-5} T + 1.8 \times 10^{-6} T^2 + 7.2 \times 10^{-8} T^3 + 6.5 \times 10^{-11} T^4$
    
Final Empirical Formula for RTSSv1
-----------------------------------
The RTSSv1 acoustic subsystem uses the following final model for computing 
speed of sound from temperature (°C) and relative humidity (%):

$S(T, H) = (331.3 + 0.606 \cdot T) \left[1 + \frac{H}{100} \left(9.66 \times 10^{-4} + 7.2 \times 10^{-5} T + 1.8 \times 10^{-6} T^2 + 7.2 \times 10^{-8} T^3 + 6.5 \times 10^{-11} T^4 \right) \right]$


Where:
- T: air temperature in °C
- H: relative humidity in percent (0 to 100)
- S(T, H): speed of sound in humid air [m/s]

Use Case and Validity
----------------------
- Valid for temperatures: 0°C ≤ T ≤ 30°C; Accuracy may decrease slightly beyond it
- Valid for relative humidity: 0% ≤ H ≤ 100%
- Assumes ambient pressure (~101.3 kPa); change in altitude can significantly cost inaccuracy
- Accuracy: ~0.1% in empirical range

This equation is ideal for low-resource embedded systems, field measurement acoustic sensing setups like RTSS-MS.

Reference
----------
- George S. K. Wong & Tony F. W. Embleton (1980). “Variation of the Speed of Sound in Air with Humidity and Temperature.”

