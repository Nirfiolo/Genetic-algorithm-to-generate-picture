#include "Individual.h"


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

    std::array<std::vector<std::vector<uint8_t>>, Color_type_count> Individual::get_points(size_t width, size_t height) const noexcept
    {
        std::vector<std::vector<uint8_t>> const r = get_value_array_from_expression(m_expressions[0], width, height);
        std::vector<std::vector<uint8_t>> const g = ga_gp::get_value_array_from_expression(m_expressions[1], width, height);
        std::vector<std::vector<uint8_t>> const b = ga_gp::get_value_array_from_expression(m_expressions[2], width, height);

        return { r, g, b };
    }

    void Individual::try_mutate(float probability) noexcept
    {
    }

    Individual Individual::cross(Individual const & first, Individual const & second, float first_advantage) noexcept
    {
        return Individual();
    }

    void Individual::generate() noexcept
    {
        auto try_generate = [](std::array<Expression, Expression_size> & expression) noexcept -> bool
        {
            for (Expression & expression : expression)
            {
                expression = get_rand_expression();
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


}
