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
        auto try_mutate = [probability](std::array<Expression, Expression_size> & expression) noexcept
        {
            for (size_t i = Expression_size - 1; i >= Expression_size / 2; --i)
            {
                if (get_bool_with_probability(probability))
                {
                    expression[i] = get_rand_no_param_expression();
                }
            }

            for (size_t i = Expression_size / 2 - 1; i > 0; --i)
            {
                if (get_bool_with_probability(probability))
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
            }
        };

        for (size_t i = 0; i < Color_type_count; ++i)
        {
            try_mutate(m_expressions[i]);
        }
    }

    Individual Individual::cross(Individual const & first, Individual const & second, float first_advantage) noexcept
    {
        auto cross = [first_advantage](std::array<Expression, Expression_size> & new_expression,
            std::array<Expression, Expression_size> const & first,
            std::array<Expression, Expression_size> const & second) noexcept
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
        };

        std::array<std::array<Expression, Expression_size>, Color_type_count> new_expressions{};

        for (size_t i = 0; i < Color_type_count; ++i)
        {
            cross(new_expressions[i], first.m_expressions[i], second.m_expressions[i]);
        }

        return Individual{ new_expressions };
    }

    void Individual::generate() noexcept
    {
        for (size_t i = 0; i < Color_type_count; ++i)
        {
            m_expressions[i] = get_rand_expression();
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
