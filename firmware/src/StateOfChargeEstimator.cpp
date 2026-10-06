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

}

float StateOfChargeEstimator::estimate(float batteryVoltage) const
{
    if (batteryVoltage <= SOC_CURVE[0].voltage)
    {
        return 0.0f;
    }

    if (batteryVoltage >= SOC_CURVE[SOC_CURVE_SIZE - 1].voltage)
    {
        return 100.0f;
    }

    for (int i = 0; i < SOC_CURVE_SIZE - 1; i++)
    {
        const SocPoint& lower = SOC_CURVE[i];
        const SocPoint& upper = SOC_CURVE[i + 1];

        if (batteryVoltage >= lower.voltage &&
            batteryVoltage <= upper.voltage)
        {
            const float fraction =
                (batteryVoltage - lower.voltage) /
                (upper.voltage - lower.voltage);

            return lower.percent +
                   fraction * (upper.percent - lower.percent);
        }
    }

    return 0.0f;
}
