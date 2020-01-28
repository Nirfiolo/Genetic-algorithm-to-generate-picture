#include "Expression.h"


#include <iostream>


namespace ga_gp
{
    bool is_correct_expression(std::array<Expression, Expression_size> const & expression, size_t & current) noexcept
    {
        if (current >= Expression_size)
        {
            return false;
        }

        Expression const current_expression = expression[current];

        ++current;


        switch (current_expression)
        {
        case Expression::One:
        case Expression::Two:

        case Expression::X:
        case Expression::Y:
            return true;

        case Expression::Sin:
        case Expression::Cos:
        case Expression::Log:
        case Expression::Atan:
        {
            bool const result = is_correct_expression(expression, current);
            return result;
        }

        case Expression::Sum:
        case Expression::Diff:
        case Expression::Mult:
        {
            bool const first = is_correct_expression(expression, current);
            bool const second = is_correct_expression(expression, current);
            return first & second;
        }

        default:
            std::cout << "Error: " << "Undefine Expression" << '\n';
            return false;
            break;
        }
    }

    Expression get_rand_expression() noexcept
    {
        uint8_t const number = static_cast<uint8_t>(rand() % static_cast<uint8_t>(Expression::Size));
        return static_cast<Expression>(number);
    }

    bool is_correct_expression(std::array<Expression, Expression_size> const & expression) noexcept
    {
        size_t current = 0;
        return is_correct_expression(expression, current);
    }


    float get_value(std::array<Expression, Expression_size> const & expression, size_t & current, float x, float y) noexcept
    {
        if (current >= Expression_size)
        {
            std::cout << "Error: " << "Out of range" << '\n';
            return 0.f;
        }


        Expression const current_expression = expression[current];
        
        ++current;

        auto get_one_value = [&expression, &current, &x, &y]() noexcept -> float
        {
            return get_value(expression, current, x, y);
        };

        auto get_two_value = [&expression, &current, &x, &y]() noexcept -> std::pair<float, float>
        {
            float const a = get_value(expression, current, x, y);
            float const b = get_value(expression, current, x, y);

            return { a, b };
        };

        switch (current_expression)
        {
        case Expression::One: return 1.f;
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
            return log(abs(current_value) + 1.f);
        }
        case Expression::Atan:
        {
            float const current_value = get_one_value();
            return atan(current_value);
        }

        default:
            std::cout << "Error: " << "Undefine Expression" << '\n';
            return 0.f;
            break;
        }
    }


    float get_value(std::array<Expression, Expression_size> const & expression, float x, float y) noexcept
    {
        size_t current = 0;
        return get_value(expression, current, x, y);
    }



    std::vector<std::vector<uint8_t>> get_value_array_from_expression(std::array<Expression, Expression_size> const & expression, size_t width, size_t height) noexcept
    {
        std::vector<std::vector<uint8_t>> result(height, std::vector<uint8_t>(width, Max_color));
        std::vector<std::vector<float>> value_array(height, std::vector<float>(width, 0.f));

        float min_value = INFINITY;
        float max_value = -INFINITY;

        float y = 0.f;
        for (size_t i = 0; i < height; ++i, y += 1.f)
        {
            float x = 0.f;
            for (size_t j = 0; j < width; ++j, x += 1.f)
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