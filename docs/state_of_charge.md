# State-of-Charge Estimation

## Overview

BattSense Rev A estimates the state of charge (SoC) of a single-cell lithium-ion battery using the battery voltage measured by the `BatteryMonitor` module.

The estimator uses a tabular open-circuit-voltage (OCV) versus state-of-charge relationship and piecewise-linear interpolation between adjacent lookup-table points.

The resulting percentage is an **estimated state of charge** and should not be interpreted as a precision measurement of remaining battery capacity.

---

## System Architecture

The state-of-charge estimator is intentionally separated from the battery measurement hardware and firmware.

```text
18650 Battery
      |
      v
Analog Front End
      |
      v
BatteryMonitor
      |
      | Battery Voltage
      v
StateOfChargeEstimator
      |
      | Estimated SoC (%)
      v
DisplayManager
      |
      v
OLED Display
```

Each module has a separate responsibility:

- `BatteryMonitor` measures battery voltage.
- `StateOfChargeEstimator` converts battery voltage into an estimated SoC.
- `DisplayManager` will present the resulting information to the user.

This separation allows the battery measurement, SoC model, and user interface to evolve independently.

---

# Rev A Estimation Method

BattSense Rev A uses:

> **OCV-SoC lookup table + piecewise-linear interpolation**

A lookup-table approach was selected because the lithium-ion voltage-versus-state-of-charge relationship is nonlinear.

A simple linear relationship such as:

\[
3.0V = 0\%
\]

and:

\[
4.2V = 100\%
\]

would incorrectly assume that battery voltage changes linearly with state of charge.

Instead, BattSense stores several OCV-SoC operating points and interpolates between neighboring points.

---

# OCV-SoC Lookup Table

BattSense Rev A uses the following generic lithium-ion OCV-SoC reference model:

| OCV (V) | SoC (%) |
| ---: | ---: |
| 3.0519 | 0 |
| 3.6594 | 10 |
| 3.7167 | 20 |
| 3.7611 | 30 |
| 3.7915 | 40 |
| 3.8275 | 50 |
| 3.8772 | 60 |
| 3.9401 | 70 |
| 4.0128 | 80 |
| 4.0923 | 90 |
| 4.1797 | 100 |

The lookup-table values are based on the example tabular OCV-SoC model presented in the following review:

> M. A. Hannan et al.,  
> "Open-Circuit Voltage Models for Battery Management Systems: A Review,"  
> *Energies*, vol. 15, no. 18, 6803, 2022.

DOI:

https://doi.org/10.3390/en15186803

The referenced work discusses tabular OCV-SoC representations and interpolation-based battery models.

BattSense uses this data as a **generic Rev A approximation** rather than as a cell-specific characterization of the particular 18650 used during development.

---

# Piecewise-Linear Interpolation

When the measured battery voltage falls between two entries in the lookup table, BattSense estimates the state of charge using linear interpolation between those neighboring points.

Consider two adjacent lookup-table points:

\[
(V_1,SOC_1)
\]

and:

\[
(V_2,SOC_2)
\]

For a measured battery voltage:

\[
V
\]

where:

\[
V_1 < V < V_2
\]

the interpolation fraction is:

\[
t =
\frac{V-V_1}
{V_2-V_1}
\]

The estimated state of charge is then:

\[
SOC =
SOC_1 +
t(SOC_2-SOC_1)
\]

---

## Example

Suppose the battery voltage is:

\[
V=3.85V
\]

The surrounding lookup-table entries are:

\[
3.8275V \rightarrow 50\%
\]

and:

\[
3.8772V \rightarrow 60\%
\]

The interpolation fraction is:

\[
t=
\frac{3.85-3.8275}
{3.8772-3.8275}
\]

\[
t\approx0.453
\]

The estimated state of charge is therefore:

\[
SOC=
50+0.453(60-50)
\]

\[
SOC\approx54.5\%
\]

Therefore:

\[
\boxed{SOC\approx55\%}
\]

The OLED display may round the calculated value to the nearest whole percentage.

---

# Boundary Conditions

The estimator must always return a value between:

\[
0\%
\]

and:

\[
100\%
\]

If the measured voltage is greater than or equal to the highest lookup-table voltage:

\[
V\ge4.1797V
\]

the estimator returns:

\[
\boxed{SOC=100\%}
\]

If the measured voltage is less than or equal to the lowest lookup-table voltage:

\[
V\le3.0519V
\]

the estimator returns:

\[
\boxed{SOC=0\%}
\]

This prevents invalid outputs such as:

```text
107%
```

or:

```text
-8%
```

---

# Firmware Architecture

The estimator is implemented as a dedicated firmware module:

```text
firmware/
├── include/
│   └── StateOfChargeEstimator.h
│
└── src/
    └── StateOfChargeEstimator.cpp
```

The public interface is intentionally small.

Conceptually:

```cpp
class StateOfChargeEstimator
{
public:
    float estimate(float batteryVoltage) const;
};
```

The module receives battery voltage as its input and returns estimated state of charge.

For example:

```text
Input:
3.85 V

Output:
~54.5 %
```

---

# Separation of Model and Algorithm

The OCV-SoC lookup table represents the **battery model**.

The interpolation function represents the **estimation algorithm**.

These are intentionally separated.

```text
OCV-SOC Lookup Table
        |
        | battery model
        v
Interpolation Algorithm
        |
        v
Estimated SoC
```

This means a future BattSense revision could replace the generic lookup table with cell-specific characterization data without redesigning the interpolation algorithm.

---

# Why SoC Estimation Is Not Part of BatteryMonitor

`BatteryMonitor` is responsible for answering:

> What is the measured battery voltage?

`StateOfChargeEstimator` is responsible for answering:

> Given this voltage and our battery model, what is the estimated state of charge?

These are separate responsibilities.

Keeping them separate prevents battery-model assumptions from becoming coupled to the ADC measurement implementation.

---

# Limitations

Voltage-based state-of-charge estimation has important limitations.

## Terminal Voltage Is Not Always OCV

Open-circuit voltage is ideally measured when the battery has been allowed to rest and transient electrochemical effects have decayed.

BattSense Rev A measures battery terminal voltage.

Under load:

\[
V_{terminal}
\neq
V_{OCV}
\]

because battery internal resistance and electrochemical polarization can cause the terminal voltage to change.

Therefore, the calculated percentage should be interpreted as an approximation.

---

## Cell Chemistry

Different lithium-ion chemistries have different OCV-SoC relationships.

Examples include:

- NMC
- NCA
- LCO
- LFP

BattSense Rev A does not attempt to automatically identify battery chemistry.

The selected model is intended as a generic lithium-ion approximation for the Rev A prototype.

---

## Temperature

Battery voltage behavior changes with temperature.

BattSense Rev A does not perform temperature compensation.

Temperature-aware SoC estimation may be investigated in a future revision.

---

## Aging

As lithium-ion cells age:

- Internal resistance increases.
- Available capacity decreases.
- Voltage behavior may change.

The Rev A estimator does not compensate for battery aging.

---

## Load Current

BattSense Rev A does not measure battery current.

Therefore it cannot compensate for load-induced voltage drop or perform coulomb counting.

This limits the accuracy of voltage-only state-of-charge estimation.

---

# Why BattSense Uses Voltage-Based SoC in Rev A

The goal of Rev A is not to implement a precision fuel gauge.

The goal is to build a modular embedded telemetry platform while demonstrating:

- Analog battery-voltage measurement
- ADC configuration and calibration
- Hardware/firmware co-design
- Lookup-table modeling
- Piecewise-linear interpolation
- Modular C++ architecture
- Automated testing
- CI-based firmware development
- Embedded user-interface development

Voltage-based SoC estimation provides an appropriate balance between useful functionality and Rev A project scope.

---

# Future Improvements

Possible future improvements include:

- Cell-specific OCV-SoC characterization
- Temperature compensation
- Battery current measurement
- Coulomb counting
- Load compensation
- Battery aging compensation
- Rest-state detection
- Combining OCV estimation with coulomb counting
- Dedicated fuel-gauge IC evaluation

These improvements are intentionally outside the scope of BattSense Rev A.

---

# References

## Primary OCV Model Reference

M. A. Hannan et al.,  
"Open-Circuit Voltage Models for Battery Management Systems: A Review,"  
*Energies*, vol. 15, no. 18, 6803, 2022.

DOI:

https://doi.org/10.3390/en15186803

---

## Additional Reference

H. He, X. Zhang, R. Xiong, Y. Xu, and H. Guo,  
"Online model-based estimation of state-of-charge and open-circuit voltage of lithium-ion batteries in electric vehicles,"  
*Energy*, vol. 39, no. 1, pp. 310-318, 2012.

DOI:

https://doi.org/10.1016/j.energy.2012.01.009

This work discusses experimentally derived OCV-SoC relationships and interpolation-based SoC estimation.

---

# Rev A Definition

BattSense reports:

> **Estimated Battery State of Charge**

It does **not** claim to provide precision remaining-capacity measurement or commercial fuel-gauge accuracy.

This distinction should be maintained in firmware documentation, the OLED user interface, and the project README.