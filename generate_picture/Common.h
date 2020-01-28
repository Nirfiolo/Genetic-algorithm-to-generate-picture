#pragma once


#include <type_traits>
#include <assert.h>


namespace ga_gp
{
    template<typename T>
    T get_rand_from_to(T from, T to) noexcept
    {
        static_assert(std::is_integral<T>::value, "Integral required.");
        
        if (to <= from)
        {
            assert(false);
            return 0;
        }

        return from + static_cast<T>(rand()) % (to - from);
    }

    template<typename T>
    T get_rand_from_zero_to(T to) noexcept
    {
        static_assert(std::is_integral<T>::value, "Integral required.");

        return static_cast<T>(rand()) % to;
    }

    float get_float_between_zero_and_one() noexcept;

    bool get_bool_with_probability(float probability) noexcept;
}