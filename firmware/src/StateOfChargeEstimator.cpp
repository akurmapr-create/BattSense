#include "StateOfChargeEstimator.h"

namespace
{

struct SocPoint
{
    float voltage;
    float percent;
};

constexpr SocPoint SOC_CURVE[] =
{
    {3.0519f,   0.0f},
    {3.6594f,  10.0f},
    {3.7167f,  20.0f},
    {3.7611f,  30.0f},
    {3.7915f,  40.0f},
    {3.8275f,  50.0f},
    {3.8772f,  60.0f},
    {3.9401f,  70.0f},
    {4.0128f,  80.0f},
    {4.0923f,  90.0f},
    {4.1797f, 100.0f}
};

constexpr int SOC_CURVE_SIZE =
    sizeof(SOC_CURVE) / sizeof(SOC_CURVE[0]);

} // namespace


float StateOfChargeEstimator::estimate(float batteryVoltage) const
{
    // Clamp voltages below the lowest lookup-table point to 0%.
    if (batteryVoltage <= SOC_CURVE[0].voltage)
    {
        return 0.0f;
    }

    // Clamp voltages above the highest lookup-table point to 100%.
    if (batteryVoltage >= SOC_CURVE[SOC_CURVE_SIZE - 1].voltage)
    {
        return 100.0f;
    }

    // Find the two lookup-table points surrounding the measured voltage.
    for (int i = 0; i < SOC_CURVE_SIZE - 1; i++)
    {
        const SocPoint& lower = SOC_CURVE[i];
        const SocPoint& upper = SOC_CURVE[i + 1];

        if (batteryVoltage >= lower.voltage &&
            batteryVoltage <= upper.voltage)
        {
            // Determine how far the measured voltage lies between
            // the two surrounding lookup-table voltages.
            const float fraction =
                (batteryVoltage - lower.voltage) /
                (upper.voltage - lower.voltage);

            // Linearly interpolate the corresponding state of charge.
            return lower.percent +
                   fraction * (upper.percent - lower.percent);
        }
    }

    // Defensive fallback. Valid voltages should never reach this point.
    return 0.0f;
}