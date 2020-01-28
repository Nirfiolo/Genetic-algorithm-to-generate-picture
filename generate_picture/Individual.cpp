#include "Individual.h"


#include "Common.h"


namespace ga_gp
{
    Individual::Individual() noexcept
    {
        generate();
    }

    Individual::Individual(std::array<std::array<Expression, Expression_size>, Color_type_count> & expressions) noexcept
        : m_expressions{ expressions }
    {
    }

    std::array<std::vector<std::vector<uint8_t>>, Color_type_count> Individual::get_points(size_t width, size_t height, float step) const noexcept
    {
        std::vector<std::vector<uint8_t>> const r = get_value_array_from_expression(m_expressions[0], width, height, step);
        std::vector<std::vector<uint8_t>> const g = get_value_array_from_expression(m_expressions[1], width, height, step);
        std::vector<std::vector<uint8_t>> const b = get_value_array_from_expression(m_expressions[2], width, height, step);

        return { r, g, b };
    }

    void Individual::try_mutate(float probability) noexcept
    {
        auto try_mutate = [probability](std::array<Expression, Expression_size> & expression) noexcept -> bool
        {
            std::array<Expression, Expression_size> new_expression = expression;

            for (size_t i = 0; i < Expression_size; ++i)
            {
                if (get_bool_with_probability(probability))
                {
                    new_expression[i] = get_rand_expression();
                }
            }

            bool const is_correct = is_correct_expression(new_expression);

            if (is_correct)
            {
                expression = new_expression;
            }

            return is_correct;
        };

        for (size_t i = 0; i < Color_type_count; ++i)
        {
            while (!try_mutate(m_expressions[i]))
            {

            }
        }
    }

    Individual Individual::cross(Individual const & first, Individual const & second, float first_advantage) noexcept
    {
        auto try_cross = [first_advantage](std::array<Expression, Expression_size> & new_expression,
            std::array<Expression, Expression_size> const & first,
            std::array<Expression, Expression_size> const & second) noexcept -> bool
        {
            for (size_t i = 0; i < Expression_size; ++i)
            {
                Expression new_expr;
                if (get_bool_with_probability(first_advantage))
                {
                    new_expr = first[i];
                }
                else
                {
                    new_expr = second[i];
                }

                new_expression[i] = new_expr;
            }

            return is_correct_expression(new_expression);
        };

        std::array<std::array<Expression, Expression_size>, Color_type_count> new_expressions{};

        for (size_t i = 0; i < Color_type_count; ++i)
        {
            while (!try_cross(new_expressions[i], first.m_expressions[i], second.m_expressions[i]))
            {

            }
        }

        return Individual{ new_expressions };
    }

    void Individual::generate() noexcept
    {
        auto try_generate = [](std::array<Expression, Expression_size> & expression) noexcept -> bool
        {
            for (Expression & expression : expression)
            {
                expression = get_rand_expression();
            }

            size_t const x_count = 10;
            size_t const y_count = 10;

            for (size_t i = 0; i < x_count; ++i)
            {
                size_t const index = get_rand_from_zero_to(Expression_size);
                expression[index] = Expression::X;
            }

            for (size_t i = 0; i < y_count; ++i)
            {
                size_t const index = get_rand_from_zero_to(Expression_size);
                expression[index] = Expression::Y;
            }

            return is_correct_expression(expression);
        };

        for (size_t i = 0; i < Color_type_count; ++i)
        {
            while (!try_generate(m_expressions[i]))
            {

            }
        }
    }

    std::ostream & operator<<(std::ostream & os, Individual const & individual) noexcept
    {
        for (size_t i = 0; i < Color_type_count; ++i)
        {
            for (size_t j = 0; j < Expression_size; ++j)
            {
                os << " " << static_cast<uint16_t>(individual.m_expressions[i][j]);
            }
        }
        return os;
    }

    std::istream & operator>>(std::istream & os, Individual & individual) noexcept
    {
        for (size_t i = 0; i < Color_type_count; ++i)
        {
            for (size_t j = 0; j < Expression_size; ++j)
            {
                uint16_t expr;
                os >> expr;
                individual.m_expressions[i][j] = static_cast<Expression>(expr);
            }
        }
        return os;
    }
}
