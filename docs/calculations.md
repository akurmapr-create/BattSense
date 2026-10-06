# Analog Front-End Calculations

This document contains the calculations used to design and revise the BattSense battery-voltage analog front end.

The original design calculations are retained to document the engineering process that led to the revised prototype.

---

# 1. System Requirements

Maximum battery voltage:

\[
V_{BAT,MAX}=4.2V
\]

ADC:

```text
ESP32 ADC1
GPIO34
12-bit resolution
6 dB attenuation
```

Target ADC source impedance:

\[
R_{SOURCE}\approx8k\Omega
\]

---

# 2. Original Analog Front-End

## Original ADC Target

The initial design targeted:

\[
V_{ADC,MAX}=2.0V
\]

at:

\[
V_{BAT,MAX}=4.2V
\]

---

## Original Divider Ratio

The voltage-divider equation is:

\[
V_{OUT}
=
V_{IN}
\left(
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
\right)
\]

Substituting the original requirements:

\[
2.0
=
4.2
\left(
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
\right)
\]

Therefore:

\[
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
=
\frac{2.0}{4.2}
\]

\[
=
\frac{10}{21}
\]

Solving:

\[
10(R_{UPPER}+R_{LOWER})
=
21R_{LOWER}
\]

\[
10R_{UPPER}+10R_{LOWER}
=
21R_{LOWER}
\]

\[
10R_{UPPER}
=
11R_{LOWER}
\]

Therefore:

\[
\boxed{
R_{UPPER}:R_{LOWER}=11:10
}
\]

---

## Original Prototype Values

The initial prototype used nominal equivalent resistances of:

\[
R_{UPPER}=16.8k\Omega
\]

\[
R_{LOWER}=15.1k\Omega
\]

The actual physical prototype used series combinations to approximate these equivalent resistances.

---

## Original Maximum ADC Voltage

\[
V_{ADC}
=
4.2
\left(
\frac{15.1}
{16.8+15.1}
\right)
\]

\[
V_{ADC}
=
4.2
\left(
\frac{15.1}
{31.9}
\right)
\]

\[
\boxed{
V_{ADC}\approx1.99V
}
\]

---

## Original Source Impedance

\[
R_{SOURCE}
=
R_{UPPER}\parallel R_{LOWER}
\]

\[
R_{SOURCE}
=
\frac{(16.8k)(15.1k)}
{16.8k+15.1k}
\]

\[
\boxed{
R_{SOURCE}\approx7.95k\Omega
}
\]

---

## Original Divider Current

\[
R_{TOTAL}
=
16.8k+15.1k
=
31.9k\Omega
\]

\[
I
=
\frac{4.2V}{31.9k\Omega}
\]

\[
\boxed{
I\approx132\mu A
}
\]

---

# 3. Initial Prototype Test Result

During breadboard validation, the ADC input approached approximately 1.9 V.

Repeated raw ADC measurements returned:

```text
4095
4095
4095
...
```

For a 12-bit ADC:

\[
ADC_{MAX}=2^{12}-1=4095
\]

The repeated maximum value indicated ADC saturation.

The analog-front-end design was therefore revised instead of attempting to compensate for the saturation in firmware.

---

# 4. Revised Analog Front-End

## Revised ADC Target

The new design target is:

\[
\boxed{
V_{ADC,MAX}\approx1.6V
}
\]

for:

\[
V_{BAT,MAX}=4.2V
\]

The required divider ratio is therefore approximately:

\[
K
=
\frac{1.6}{4.2}
\]

\[
\boxed{
K\approx0.381
}
\]

where:

\[
K=
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
\]

---

# 5. Revised Prototype Resistor Selection

Available resistor combinations produced:

\[
\boxed{
R_{UPPER}=20.7k\Omega
}
\]

\[
\boxed{
R_{LOWER}=13.0k\Omega
}
\]

Total divider resistance:

\[
R_{TOTAL}
=
20.7k+13.0k
\]

\[
\boxed{
R_{TOTAL}=33.7k\Omega
}
\]

---

# 6. Revised Divider Ratio

\[
K
=
\frac{13.0}
{20.7+13.0}
\]

\[
K
=
\frac{13.0}{33.7}
\]

\[
\boxed{
K\approx0.3858
}
\]

---

# 7. Revised Maximum ADC Voltage

At the maximum battery voltage:

\[
V_{ADC,MAX}
=
4.2(0.3858)
\]

\[
\boxed{
V_{ADC,MAX}\approx1.62V
}
\]

This is close to the revised 1.6 V design target.

---

# 8. Revised Source Impedance

The ADC sees approximately the Thevenin equivalent resistance of the divider:

\[
R_{SOURCE}
=
R_{UPPER}\parallel R_{LOWER}
\]

\[
R_{SOURCE}
=
\frac{(20.7k)(13.0k)}
{20.7k+13.0k}
\]

\[
R_{SOURCE}
=
\frac{269.1}
{33.7}
k\Omega
\]

\[
\boxed{
R_{SOURCE}\approx7.99k\Omega
}
\]

This is almost exactly the original 8 kΩ source-impedance target.

---

# 9. Revised Divider Current

At maximum battery voltage:

\[
I_{DIVIDER}
=
\frac{V_{BAT}}
{R_{TOTAL}}
\]

\[
I_{DIVIDER}
=
\frac{4.2}
{33.7k}
\]

\[
\boxed{
I_{DIVIDER}\approx125\mu A
}
\]

The revised divider therefore consumes slightly less current than the original design.

---

# 10. Battery Voltage Reconstruction

The ADC measures:

\[
V_{ADC}
=
V_{BAT}
\left(
\frac{R_{LOWER}}
{R_{UPPER}+R_{LOWER}}
\right)
\]

Solving for battery voltage:

\[
V_{BAT}
=
V_{ADC}
\left(
\frac{R_{UPPER}+R_{LOWER}}
{R_{LOWER}}
\right)
\]

Using the revised resistor values:

\[
V_{BAT}
=
V_{ADC}
\left(
\frac{20.7+13.0}
{13.0}
\right)
\]

\[
V_{BAT}
=
V_{ADC}
\left(
\frac{33.7}{13.0}
\right)
\]

Therefore:

\[
\boxed{
V_{BAT}\approx2.5923V_{ADC}
}
\]

The firmware calculates this relationship from the resistor values rather than storing `2.5923` as a hard-coded multiplier.

---

# 11. Revised Prototype ADC Test

After installing the revised divider, the following raw ADC samples were observed:

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

Unlike the original design, the ADC was no longer pinned at:

```text
4095
```

This verifies that the revised voltage divider successfully moved the ADC input away from saturation.

The sample variation will be characterized separately before deciding whether digital averaging or filtering is necessary.

---

# 12. Design Comparison

| Parameter | Original Design | Revised Design |
| :--- | ---: | ---: |
| Maximum battery voltage | 4.2 V | 4.2 V |
| ADC target | 2.0 V | ~1.6 V |
| Upper resistance | 16.8 kΩ | 20.7 kΩ |
| Lower resistance | 15.1 kΩ | 13.0 kΩ |
| Maximum ADC voltage | ~1.99 V | ~1.62 V |
| Source impedance | ~7.95 kΩ | ~7.99 kΩ |
| Divider current @ 4.2 V | ~132 µA | ~125 µA |
| Raw ADC prototype behavior | Saturated at 4095 | ~3100 counts |

---

# 13. Engineering Conclusion

The original analog front-end met its calculated divider and source-impedance requirements but produced ADC saturation during physical testing.

The revised design:

- Reduces maximum ADC input voltage.
- Preserves approximately 8 kΩ source impedance.
- Slightly reduces continuous divider current.
- Eliminates the observed raw ADC saturation.
- Maintains sufficient range for measurement of a 1S lithium-ion battery.

This revision demonstrates the complete engineering cycle:

```text
Requirements
    ↓
Design
    ↓
Calculation
    ↓
Prototype
    ↓
Test
    ↓
Failure Observation
    ↓
Root-Cause Investigation
    ↓
Design Revision
    ↓
Verification
```

Further optimization will be based on measured ADC accuracy and noise rather than assumptions.