#include "Common.h"


namespace ga_gp
{
    float get_float_between_zero_and_one() noexcept
    {
        return static_cast<float>(rand() % 10'000) / 10'000.f;
    }

    bool get_bool_with_probability(float probability) noexcept
    {
        if (probability < 0.f || probability > 1.f)
        {
            assert(false);
        }

        float new_probability = get_float_between_zero_and_one();
        return new_probability < probability;
    }


}