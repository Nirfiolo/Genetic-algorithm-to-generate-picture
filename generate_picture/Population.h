#pragma once


#include "Individual.h"


namespace ga_gp
{
    class Population
    {
    public:
        Population(size_t size) noexcept;
        Population(std::vector<Individual> const & individuals) noexcept;

        std::vector<Individual> const & get_individuals() const noexcept;

        void reroll_pair() noexcept;
        std::pair<Individual, Individual> get_pair() const noexcept;
        void submit_result(bool is_first) noexcept;


        void set_max_evolution_number(size_t max_evolution_number) noexcept;

        size_t get_evolution_number() const noexcept;

        bool go_to_next_evolution() noexcept;

    public:
        // TODO friend std::ostream & operator<<(std::ostream & os, Population const & population);

    private:
        void generate() noexcept;

        void calculate_fitness() noexcept;
        std::vector<float> get_average_deviation() const noexcept;
        std::vector<size_t> get_go_to_next_evolution_elements(std::vector<float> const & average_deviation) const noexcept;
        std::vector<size_t> get_sorted_indices_to_pair() const noexcept;
        std::vector<std::pair<size_t, size_t>> get_pairs(size_t pair_count) const noexcept;
        std::vector<Individual> get_next_population(std::vector<std::pair<size_t, size_t>> const & pairs) const noexcept;

    private:
        static constexpr size_t Fitness_scale_array_size{ 50 };
        // unnecessary now
        std::array<size_t, Fitness_scale_array_size> get_fitness_scale_array() const noexcept;

    private:
        std::vector<Individual> m_individuals{};


        std::vector<float> m_fitness{};
        std::vector<uint8_t> m_pre_fitness{};
        std::pair<size_t, size_t> m_current_pair{ 0, 0 };


        size_t m_size{ Min_size };

        size_t m_evolution_number{ 1 };

        size_t m_max_evolution_number{ 200 };

        bool m_is_end{ false };

        bool m_is_full_output{ false };
        bool m_is_special_output{ false };

    private:
        static constexpr size_t Max_size{ 500 };
        static constexpr size_t Min_size{ 2 };
        static constexpr float To_next_evolution_coefficient{ 0.25f };
        static constexpr float To_new_pairs_coefficient{ 0.368f };
        static constexpr float Mutate_coefficient{ 0.10f };
    };

}