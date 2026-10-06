#include <unity.h>
#include "StateOfChargeEstimator.h"

StateOfChargeEstimator estimator;

void test_lower_voltage_clamps_to_zero()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        0.0f,
        estimator.estimate(2.90f)
    );
}

void test_upper_voltage_clamps_to_100()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        100.0f,
        estimator.estimate(4.30f)
    );
}

void test_exact_50_percent_point()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        50.0f,
        estimator.estimate(3.8275f)
    );
}

void test_exact_100_percent_point()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        100.0f,
        estimator.estimate(4.1797f)
    );
}

void test_interpolation_between_50_and_60_percent()
{
    const float result = estimator.estimate(3.85f);

    TEST_ASSERT_FLOAT_WITHIN(
        0.1f,
        54.5f,
        result
    );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_lower_voltage_clamps_to_zero);
    RUN_TEST(test_upper_voltage_clamps_to_100);
    RUN_TEST(test_exact_50_percent_point);
    RUN_TEST(test_exact_100_percent_point);
    RUN_TEST(test_interpolation_between_50_and_60_percent);

    return UNITY_END();
}
