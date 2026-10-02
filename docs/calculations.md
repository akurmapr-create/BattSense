# Analog Front-End Calculations

This document contains the engineering calculations used to design the BattSense analog front-end.

---

# Design Specifications

Maximum battery voltage:

\[
V_{BAT}=4.2V
\]

Target ADC voltage:

\[
V_{ADC}=2.0V
\]

ADC attenuation:

**6 dB**

Target source impedance:

\[
R_{SOURCE}=8k\Omega
\]

---

# Voltage Divider Ratio

The voltage divider equation is

\[
V_{OUT}=V_{IN}
\left(
\frac{R_2}{R_1+R_2}
\right)
\]

Substituting the design values:

\[
2.0
=
4.2
\left(
\frac{R_2}{R_1+R_2}
\right)
\]

Divide both sides by 4.2:

\[
\frac{2.0}{4.2}
=
\frac{R_2}{R_1+R_2}
\]

Simplify:

\[
\frac{10}{21}
=
\frac{R_2}{R_1+R_2}
\]

Cross multiply:

\[
10(R_1+R_2)=21R_2
\]

Expand:

\[
10R_1+10R_2=21R_2
\]

Rearrange:

\[
10R_1=11R_2
\]

Result:

\[
R_1:R_2=11:10
\]

---

# Source Impedance

The source impedance seen by the ADC is the Thevenin equivalent resistance of the divider.

\[
R_{SOURCE}=R_1\parallel R_2
\]

Expanding:

\[
R_{SOURCE}
=
\frac{R_1R_2}{R_1+R_2}
\]

Using the resistor ratio:

\[
R_1=11k
\]

\[
R_2=10k
\]

Substitute:

\[
R_{SOURCE}
=
\frac{110k^2}{21k}
=
5.238k
\]

Set the desired source impedance:

\[
5.238k=8k\Omega
\]

Solve:

\[
k=1.527k\Omega
\]

Resulting resistor values:

\[
R_1
=
11k
=
16.8k\Omega
\]

\[
R_2
=
10k
=
15.2k\Omega
\]

---

# Divider Current

Total resistance:

\[
R_{TOTAL}
=
16.8k\Omega+15.2k\Omega
=
32k\Omega
\]

Current through the divider:

\[
I=\frac{V}{R}
\]

Substitute:

\[
I
=
\frac{4.2V}{32k\Omega}
\]

Result:

\[
I
=
131.25\mu A
\]

---

## Final Component Selection (Based on available resistors)

Selected resistor values:

- R₁ = 16.6 kΩ
- R₂ = 14.87 kΩ

Actual divider ratio:

R₂ / (R₁ + R₂) = 15.1 / 31.9 = 0.473

Maximum ADC voltage at 4.2 V battery:

≈1.99 V

Source impedance:

≈7.95 kΩ

Divider current:

≈131.7 µA

# Summary

| Parameter | Result |
| :-------- | -----: |
| Maximum Battery Voltage | 4.2 V |
| Target ADC Voltage | 2.0 V |
| Divider Ratio | 11 : 10 |
| Target Source Impedance | 8 kΩ |
| Calculated R₁ | 16.8 kΩ |
| Calculated R₂ | 15.2 kΩ |
| Total Divider Resistance | 32 kΩ |
| Divider Current | 131.25 µA |
