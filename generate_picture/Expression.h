#pragma once


#include <array>
#include <vector>

#include "Constants.h"


namespace ga_gp
{
    enum class Expression : uint8_t
    {
        Size = 11,

        One = 0,
        Two = 1,

        X = 2,
        Y = 3,

        Sum = 4,
        Diff = 5,
        Mult = 6,

        Sin = 7,
        Cos = 8,
        Log = 9,   // log(|x| + 1.f)
        Atan = 10

        // TODO 
        // 1 / (1 + e ^ -x)
        // relu 
    };

    static constexpr size_t Expression_size{ 10 };


    Expression get_rand_expression() noexcept;

    bool is_correct_expression(std::array<Expression, Expression_size> const & expression) noexcept;

    float get_value(std::array<Expression, Expression_size> const & expression, float x, float y) noexcept;

    std::vector<std::vector<uint8_t>> get_value_array_from_expression(std::array<Expression, Expression_size> const & expression, size_t width, size_t height) noexcept;
}