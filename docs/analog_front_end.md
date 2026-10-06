# Analog Front-End Design

## Overview

The BattSense analog front-end measures the terminal voltage of a single-cell (1S) 18650 lithium-ion battery using the ESP32 ADC.

Because the maximum battery voltage exceeds the selected ADC measurement range, a passive resistive voltage divider scales the battery voltage before it reaches the ESP32.

> **Note**
>
> BattSense is a battery telemetry platform, **not** a Battery Management System (BMS). The analog front-end performs voltage measurement only. It does not provide charging control, cell balancing, over-current protection, or battery disconnect functionality.

---

## Requirements

| Requirement | Value |
| :--- | ---: |
| Battery chemistry | 1S Li-ion (18650) |
| Maximum battery voltage | 4.2 V |
| Microcontroller | ESP32-WROOM-32 DevKit |
| ADC peripheral | ADC1 |
| ADC input | GPIO34 |
| ADC resolution | 12 bit |
| ADC attenuation | 6 dB |
| Revised maximum ADC target | ~1.6 V |
| Target ADC source impedance | ~8 kΩ |

---

## System Architecture

```text
18650 Battery
      │
      ▼
Voltage Divider
      │
      ▼
ADC_SENSE
      │
      ▼
GPIO34 / ADC1
      │
      ▼
BatteryMonitor
      │
      ▼
Battery Voltage
      │
      ▼
Future Telemetry System
```

The ESP32 development board is powered independently through USB during Rev A testing. The 18650 cell is used as the measured source and does not power the ESP32.

The battery and ESP32 share a common ground so that the ADC measurement has the correct voltage reference.

---

## ADC Configuration

BattSense uses:

- ADC1
- GPIO34
- 12-bit ADC resolution
- 6 dB attenuation

ADC1 was selected to provide a dedicated analog measurement path and avoid the resource-sharing limitations associated with ADC2 when ESP32 wireless functionality is used.

The 6 dB attenuation setting was retained after prototype testing, but the analog front-end scaling was revised to keep the ADC input comfortably within the recommended measurement range.

---

# Analog Front-End Revision History

## Initial Design

The original analog front-end targeted:

\[
V_{BAT,MAX}=4.2V
\]

and:

\[
V_{ADC,MAX}=2.0V
\]

The resulting divider used nominal equivalent resistances of:

\[
R_{UPPER}=16.8k\Omega
\]

\[
R_{LOWER}=15.1k\Omega
\]

This produced a theoretical maximum ADC voltage of approximately:

\[
V_{ADC,MAX}\approx1.99V
\]

The design also maintained a source impedance of approximately 8 kΩ and a divider current of approximately 132 µA.

### Prototype Result

During breadboard testing, the ADC input was approximately 1.9 V and repeated calls to `analogRead()` returned:

```text
4095
4095
4095
...
```

A value of 4095 represents the maximum possible output of a 12-bit ADC and indicated that the ADC was saturating.

This demonstrated that the original 2.0 V target was too aggressive for reliable operation with the ESP32 ADC configured for 6 dB attenuation.

Rather than compensating for the behavior in firmware, the analog front-end was redesigned.

---

# Revised Analog Front-End

## Revised ADC Target

The maximum ADC target was reduced from approximately:

\[
2.0V
\]

to:

\[
\boxed{1.6V}
\]

at the maximum expected battery voltage of:

\[
4.2V
\]

This provides additional headroom below the upper portion of the usable ADC measurement range.

---

## Final Prototype Divider

The revised breadboard prototype uses equivalent resistances of:

\[
\boxed{R_{UPPER}=20.7k\Omega}
\]

and:

\[
\boxed{R_{LOWER}=13.0k\Omega}
\]

The divider topology is:

```text
VBAT
 │
 │
20.7 kΩ
 │
 ├──────── ADC_SENSE ─────── GPIO34
 │
13.0 kΩ
 │
GND
```

If an equivalent resistance is constructed from multiple physical resistors, the KiCad schematic documents the individual physical components used in the prototype.

---

## Revised Maximum ADC Voltage

The divider relationship is:

\[
V_{ADC}
=
V_{BAT}
\left(
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
\right)
\]

At the maximum battery voltage:

\[
V_{ADC}
=
4.2
\left(
\frac{13.0}
{20.7+13.0}
\right)
\]

Therefore:

\[
\boxed{V_{ADC,MAX}\approx1.62V}
\]

This is significantly lower than the original approximately 2.0 V target and prevents the ADC from operating near the saturation condition observed during initial testing.

---

## Source Impedance

The source impedance presented to the ADC is approximately the Thevenin equivalent resistance of the divider:

\[
R_{SOURCE}
=
R_{UPPER}\parallel R_{LOWER}
\]

For the revised divider:

\[
R_{SOURCE}
=
20.7k\Omega\parallel13.0k\Omega
\]

which gives:

\[
\boxed{R_{SOURCE}\approx7.99k\Omega}
\]

This remains extremely close to the original design target of approximately 8 kΩ.

---

## Divider Current

At the maximum battery voltage:

\[
R_{TOTAL}
=
20.7k\Omega+13.0k\Omega
=
33.7k\Omega
\]

The divider current is therefore:

\[
I_{DIVIDER}
=
\frac{4.2V}{33.7k\Omega}
\]

\[
\boxed{I_{DIVIDER}\approx125\mu A}
\]

The revised design therefore slightly reduces continuous divider current compared with the original prototype.

---

# Prototype Validation

## Initial Prototype

The initial divider produced an ADC input near approximately 1.9 V during testing.

The ESP32 repeatedly returned a raw ADC value of:

```text
4095
```

indicating ADC saturation.

---

## Revised Prototype

After changing the divider to approximately:

- 20.7 kΩ upper resistance
- 13.0 kΩ lower resistance

the raw ADC measurements were approximately:

```text
3109
3111
3103
3107
3113
3110
3114
3098
3093
3099
3145
3088
3098
3149
3119
3120
3107
3152
```

The ADC was therefore no longer saturated.

This experimentally verified that the revised analog-front-end scaling moved the ADC input back into a usable measurement region.

The variation between individual ADC samples will be characterized separately before deciding whether averaging or digital filtering is necessary.

---

# Firmware Interface

The analog front-end is represented in firmware by the `BatteryMonitor` module.

The module:

- Configures GPIO34 as the battery ADC input.
- Configures 12-bit ADC resolution.
- Configures 6 dB attenuation.
- Acquires raw ADC measurements for diagnostics.
- Obtains ADC-pin voltage measurements.
- Applies voltage-divider compensation.
- Returns the estimated battery terminal voltage.

The firmware uses the actual analog-front-end resistor values rather than a hard-coded divider multiplier.

Conceptually:

\[
V_{BAT}
=
V_{ADC}
\left(
\frac{R_{UPPER}+R_{LOWER}}
{R_{LOWER}}
\right)
\]

This keeps the firmware implementation directly traceable to the hardware design.

---

# Design Tradeoffs

| Decision | Benefit | Tradeoff |
| :--- | :--- | :--- |
| ADC1 / GPIO34 | Suitable dedicated analog input | Consumes one ADC1 channel |
| 6 dB attenuation | Retains selected ADC configuration | Requires external voltage scaling |
| ~1.6 V maximum target | Prevents observed saturation and adds headroom | Uses less of the theoretical ADC range |
| ~8 kΩ source impedance | Supports reliable ADC sampling | Requires relatively low divider resistance |
| Passive divider | Simple and inexpensive | Continuously draws battery current |
| ~125 µA divider current | Acceptable for Rev A | Too high for future ultra-low-power designs |

---

# Future Improvements

Potential future improvements include:

- Characterizing ADC measurement error across multiple battery voltages.
- Quantifying sample-to-sample ADC noise.
- Determining whether sample averaging improves measurement repeatability.
- Characterizing divider tolerance using higher-accuracy test equipment.
- Adding a switched divider for lower sleep current.
- Evaluating an ADC input capacitor if higher divider resistance is used.
- Designing a dedicated BattSense PCB after breadboard validation is complete.

Filtering and averaging will only be introduced after measurement data demonstrates a need for them.

---

# References

- ESP32-WROOM-32 Datasheet
- ESP32 Technical Reference Manual
- ESP-IDF ADC Documentation
- Arduino-ESP32 ADC API Documentation