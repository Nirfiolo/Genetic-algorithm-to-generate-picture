#pragma once


#include "Expression.h"


namespace ga_gp
{
    class Individual
    {
    public:
        Individual() noexcept;
        Individual(std::array<std::array<Expression, Expression_size>, Color_type_count> & expressions) noexcept;


        std::array<std::vector<std::vector<uint8_t>>, Color_type_count> get_points(size_t width, size_t height) const noexcept;

        void try_mutate(float probability) noexcept;

    public:
        static Individual cross(Individual const & first, Individual const & second, float first_advantage) noexcept;
        // TODO cross with two child

    public:
        // TODO friend std::ostream & operator<<(std::ostream & os, Individual const & individual) noexcept;


    private:
        void generate() noexcept;

    private:
        std::array<std::array<Expression, Expression_size>, Color_type_count> m_expressions{};

    private:
        static constexpr size_t Min_size{ 2 };
    };
}
