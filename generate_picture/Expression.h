#pragma once


#include <array>
#include <vector>

#include "Constants.h"


namespace ga_gp
{
    enum class Expression : uint8_t
    {
        BeginNoParam = 1,
        //One = 1,
        Two = 2,

        X = 3,
        Y = 4,
        EndNoParam = 5,

        BeginDoubleParam = 10,
        Sum = 11,
        Diff = 12,
        Mult = 13,
        Div = 14,
        EndDoubleParam = 15,

        BeginSingleParam = 20,
        Sin = 21,
        Cos = 22,
        Log = 23,   // log(|x| + 1.f)
        Atan = 24,
        Exp = 25,
        Logis_1 = 26,    // 1 / (1 + e ^ -x)
        Sqrt = 27,
        Fun_1 = 28,     // sin(x) * x
        Fun_2 = 29,     // sin(x) / x
        EndSingleParam = 30


        // TODO 
        // 1 / (1 + e ^ -x)
        // relu 
    };

    static constexpr size_t Expression_size{ 128 };

    Expression get_rand_single_param_expression() noexcept;
    Expression get_rand_double_param_expression() noexcept;
    Expression get_rand_no_param_expression() noexcept;

    std::array<Expression, Expression_size> get_rand_expression() noexcept;

    float get_value(std::array<Expression, Expression_size> const & expression, float x, float y) noexcept;

    std::vector<std::vector<uint8_t>> get_value_array_from_expression(std::array<Expression, Expression_size> const & expression, size_t width, size_t height, float step = 1.f) noexcept;
}