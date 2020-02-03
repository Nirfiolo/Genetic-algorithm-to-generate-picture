#include "Expression.h"

#include "Common.h"

#include <iostream>


namespace ga_gp
{
    Expression get_rand_single_param_expression() noexcept
    {
        uint8_t const begin = static_cast<uint8_t>(Expression::BeginSingleParam) + 1;
        uint8_t const end = static_cast<uint8_t>(Expression::EndSingleParam);
        uint8_t const number = get_rand_from_to(begin, end);
        return static_cast<Expression>(number);
    }

    Expression get_rand_double_param_expression() noexcept
    {
        uint8_t const begin = static_cast<uint8_t>(Expression::BeginDoubleParam) + 1;
        uint8_t const end = static_cast<uint8_t>(Expression::EndDoubleParam);
        uint8_t const number = get_rand_from_to(begin, end);
        return static_cast<Expression>(number);
    }

    Expression get_rand_no_param_expression() noexcept
    {
        uint8_t const begin = static_cast<uint8_t>(Expression::BeginNoParam) + 1;
        uint8_t const end = static_cast<uint8_t>(Expression::EndNoParam);
        uint8_t const number = get_rand_from_to(begin, end);
        return static_cast<Expression>(number);
    }

    std::array<Expression, Expression_size> get_rand_expression() noexcept
    {
        std::array<Expression, Expression_size> expression;

        for (size_t i = Expression_size - 1; i >= Expression_size / 2; --i)
        {
            expression[i] = get_rand_no_param_expression();
        }

        for (size_t i = Expression_size / 2 - 1; i > 0; --i)
        {
            if (get_bool_with_probability(0.6f))
            {
                expression[i] = get_rand_double_param_expression();
            }
            else
            {
                expression[i] = get_rand_single_param_expression();
            }
        }

        return expression;
    }

    float get_value(std::array<Expression, Expression_size> const & expression, size_t current, float x, float y) noexcept
    {
        if (current >= Expression_size)
        {
            std::cout << "Error: " << "Out of range" << '\n';
            return 0.f;
        }

        Expression const current_expression = expression[current];
        

        auto get_one_value = [&expression, current, x, y]() noexcept -> float
        {
            return get_value(expression, 2 * current, x, y);
        };

        auto get_two_value = [&expression, current, x, y]() noexcept -> std::pair<float, float>
        {
            float const a = get_value(expression, 2 * current, x, y);
            float const b = get_value(expression, 2 * current + 1, x, y);

            return { a, b };
        };

        switch (current_expression)
        {
        //case Expression::One: return 1.f;
        case Expression::Two: return 2.f;
        
        case Expression::X: return x;
        case Expression::Y: return y;

        case Expression::Sum:
        {
            std::pair<float, float> const current_value = get_two_value();
            return current_value.first + current_value.second;
        }
        case Expression::Diff:
        {
            std::pair<float, float> const current_value = get_two_value();
            return current_value.first - current_value.second;
        }
        case Expression::Mult:
        {
            std::pair<float, float> const current_value = get_two_value();
            return current_value.first * current_value.second;
        }
        case Expression::Div:
        {
            std::pair<float, float> current_value = get_two_value();

            float const eps = 0.0001f;
            
            if (abs(current_value.second) < eps)
            {
                current_value.second = (current_value.second < 0.f ? -1.f : 1.f) * eps;
            }

            return current_value.first / current_value.second;
        }

        case Expression::Sin:
        {
            float const current_value = get_one_value();
            return sinf(current_value);
        }
        case Expression::Cos:
        {
            float const current_value = get_one_value();
            return cosf(current_value);
        }
        case Expression::Log:
        {
            float const current_value = get_one_value();
            return logf(abs(current_value) + 1.f);
        }
        case Expression::Atan:
        {
            float const current_value = get_one_value();
            return atanf(current_value);
        }
        case Expression::Exp:
        {
            float const current_value = get_one_value();
            return expf(current_value);
        }
        case Expression::Logis_1:
        {
            float const current_value = get_one_value();
            return 1.f / (1.f + expf(-current_value));
        }
        case Expression::Sqrt:
        {
            float const current_value = get_one_value();
            return sqrtf(abs(current_value));
        }
        case Expression::Fun_1:
        {
            float const current_value = get_one_value();
            return sinf(current_value) * current_value;
        }
        case Expression::Fun_2:
        {
            float const current_value = get_one_value();
            return sinf(current_value) / current_value;
        }

        default:
            std::cout << "Error: " << "Undefine Expression" << '\n';
            return 0.f;
            break;
        }
    }


    float get_value(std::array<Expression, Expression_size> const & expression, float x, float y) noexcept
    {
        return get_value(expression, 1, x, y);
    }



    std::vector<std::vector<uint8_t>> get_value_array_from_expression(std::array<Expression, Expression_size> const & expression, size_t width, size_t height, float step) noexcept
    {
        std::vector<std::vector<uint8_t>> result(height, std::vector<uint8_t>(width, Max_color));
        std::vector<std::vector<float>> value_array(height, std::vector<float>(width, 0.f));

        float min_value = INFINITY;
        float max_value = -INFINITY;

        float y = -0.5f * step * static_cast<float>(height);
        for (size_t i = 0; i < height; ++i, y += step)
        {
            float x = -0.5f * step * static_cast<float>(width);
            for (size_t j = 0; j < width; ++j, x += step)
            {
                float const current = get_value(expression, x, y);

                value_array[i][j] = current;

                min_value = std::min(min_value, current);
                max_value = std::max(max_value, current);
            }
        }

        if (max_value - min_value < FLT_EPSILON)
        {
            min_value -= 1.f;
        }

        for (size_t i = 0; i < height; ++i)
        {
            for (size_t j = 0; j < width; ++j)
            {
                float current = value_array[i][j];

                current = (current - min_value) * (Max_color_float / (max_value - min_value));

                uint8_t current_result;

                if (current <= FLT_EPSILON)
                {
                    current_result = 0;
                }
                else if (current >= Max_color_float - FLT_EPSILON)
                {
                    current_result = Max_color;
                }
                else
                {
                    current_result = static_cast<uint8_t>(current);
                }

                result[i][j] = current_result;
            }
        }

        return result;
    }
}