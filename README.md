## Real Time Speed of Sound (RTSS) - Monitoring Setup

| Category                | RTSSv0.7                                                    | RTSS-MS (v1.0)                             | Commentary                                     |
| ----------------------- | ----------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------- |
| **Objective**           | Compute and compare speed of sound using sensors            | Same core objective                        | Conceptually same - but implementation matured |
| **Theoretical Model**   | Used simplified empirical model: `331.3 + 0.606T + 0.0124H` | Uses Wong–Embleton (4th degree polynomial) | v1 adds scientific rigor and accuracy          |
| **Timing**              | `esp_timer_get_time()` via `IRAM_ATTR` ISR                  | Same, retained                             | No change — already well-implemented in v0.7   |
| **Vertical Correction** | Used `D_corrected = sqrt(D^2 + 1.05^2)`                     | Same                                       | Consistent geometric correction in both        |
| **Modularity**          | Monolithic code                                             | Modularized into `.h/.cpp` structure       | Easier to extend, debug, document              |
| **Validation Logic**    | Simple comparison                                           | Uses `alignmentCheck()` with upload gating | Adds logic layer before data goes out          |
| **Data Handling**       | Serial only                                                 | Serial + ThingSpeak every 15s              | MS adds live data publishing                   |
| **Upload Trigger**      | None — every cycle logs                                     | Conditional on alignment check             | Helps filter out misreads/noise                |
| **Code Style**          | Working code with hardcoding & quick print                  | Cleaner names, portable                    | More reusable and readable                     |
