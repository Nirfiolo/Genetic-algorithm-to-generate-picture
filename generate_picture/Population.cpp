#include "Population.h"

#include "Common.h"


namespace ga_gp
{
    Population::Population(size_t size) noexcept
        : m_size{ size }
    {
        generate();
    }

    Population::Population(std::vector<Individual> const & individuals) noexcept
        : m_individuals{ individuals }, m_size{ individuals.size() }
    {
    }

    std::vector<Individual> const & Population::get_individuals() const noexcept
    {
        return m_individuals;
    }

    void Population::reroll_pair() noexcept
    {
        size_t const first = get_rand_from_zero_to(m_size);

        size_t second = get_rand_from_zero_to(m_size);;
        while (second == first)
        {
            second = get_rand_from_zero_to(m_size);
        }

        m_current_pair = { first, second };
    }

    std::pair<Individual, Individual> Population::get_pair() const noexcept
    {
        return { m_individuals[m_current_pair.first], m_individuals[m_current_pair.second] };
    }

    void Population::submit_result(bool is_first) noexcept
    {
        if (is_first)
        {
            m_pre_fitness[m_current_pair.first] += 100;
        }
        else
        {
            m_pre_fitness[m_current_pair.second] += 100;
        }
    }

    void Population::set_max_evolution_number(size_t max_evolution_number) noexcept
    {
        m_max_evolution_number = max_evolution_number;
    }

    size_t Population::get_evolution_number() const noexcept
    {
        return m_evolution_number;
    }

    bool Population::go_to_next_evolution() noexcept
    {
        if (m_evolution_number >= m_max_evolution_number)
        {
            return false;
        }

        calculate_fitness();

        std::vector<float> const average_deviation = get_average_deviation();
        std::vector<size_t> const go_to_next_evolution_elements = get_go_to_next_evolution_elements(average_deviation);
        std::vector<std::pair<size_t, size_t>> const pairs = get_pairs(m_size - go_to_next_evolution_elements.size());

        std::vector<Individual> next_population = get_next_population(pairs);

        for (size_t index : go_to_next_evolution_elements)
        {
            next_population.push_back(std::move(m_individuals[index]));
        }

        for (Individual & individual : next_population)
        {
            individual.try_mutate(Mutate_coefficient);
        }

        m_individuals = std::move(next_population);

        calculate_fitness();

        ++m_evolution_number;

        for (size_t i = 0; i < m_size; ++i)
        {
            m_pre_fitness[i] = 1;
        }

        return true;
    }

    void Population::generate() noexcept
    {
        m_individuals.resize(m_size);
        m_pre_fitness.resize(m_size);

        for (size_t i = 0; i < m_size; ++i)
        {
            m_individuals[i] = Individual{};
            m_pre_fitness[i] = 1;
        }
    }

    void Population::calculate_fitness() noexcept
    {
        m_fitness.resize(m_size);

        float sum = 0.f;
        for (size_t i = 0; i < m_size; ++i)
        {
            float current_fitness = static_cast<float>(m_pre_fitness[i]);
            current_fitness = std::max(FLT_EPSILON, current_fitness);

            current_fitness = 1.f / current_fitness;
            sum += current_fitness;

            m_fitness[i] = current_fitness;
        }
        for (float & current_fitness : m_fitness)
        {
            current_fitness = current_fitness / sum;
        }
    }

    std::vector<float> Population::get_average_deviation() const noexcept
    {
        std::vector<float> average_deviation{};

        float sum = 0.f;

        for (float current_fitness : m_fitness)
        {
            sum += current_fitness;
        }

        sum /= static_cast<float>(m_size);

        for (float current_fitness : m_fitness)
        {
            average_deviation.push_back(abs(current_fitness - sum));
        }

        return average_deviation;
    }

    std::vector<size_t> Population::get_go_to_next_evolution_elements(std::vector<float> const & average_deviation) const noexcept
    {
        std::vector<size_t> indices{};
        for (size_t i = 0; i < m_size; ++i)
        {
            if (average_deviation[i] > To_next_evolution_coefficient)
            {
                indices.push_back(i);
            }
        }
        return indices;
    }

    std::vector<size_t> Population::get_sorted_indices_to_pair() const noexcept
    {
        std::vector<size_t> sorted_indices_to_pair{};

        std::vector<std::pair<float, size_t>> fitness_to_index{};

        for (size_t i = 0; i < m_size; ++i)
        {
            fitness_to_index.push_back({ m_fitness[i], i });
        }

        std::sort(fitness_to_index.begin(), fitness_to_index.end(), std::greater<std::pair<float, size_t>>());

        float const to_pairs_size_float = To_new_pairs_coefficient * static_cast<float>(m_size);
        size_t const to_pairs_size = std::max(Min_size, static_cast<size_t>(to_pairs_size_float));

        for (size_t i = 0; i < to_pairs_size; ++i)
        {
            sorted_indices_to_pair.push_back(fitness_to_index[i].second);
        }

        return sorted_indices_to_pair;
    }

    std::vector<std::pair<size_t, size_t>> Population::get_pairs(size_t pair_count) const noexcept
    {
        std::vector<std::pair<size_t, size_t>> pairs{};

        std::vector<size_t> sorted_indices_to_pair = get_sorted_indices_to_pair();

        for (size_t i = 0; i < pair_count; ++i)
        {
            auto get_index = [&sorted_indices_to_pair]() noexcept -> size_t
            {
                size_t const index = get_rand_from_zero_to(sorted_indices_to_pair.size());
                return sorted_indices_to_pair[index];
            };

            size_t const first = get_index();

            size_t second = get_index();
            while (second == first)
            {
                second = get_index();
            }

            pairs.push_back({ first, second });
        }

        return pairs;
    }

    std::vector<Individual> Population::get_next_population(std::vector<std::pair<size_t, size_t>> const & pairs) const noexcept
    {
        std::vector<Individual> next_population{};

        for (size_t i = 0; i < pairs.size(); ++i)
        {
            size_t const first = pairs[i].first;
            size_t const second = pairs[i].second;

            float const first_advantage = m_fitness[first] / (m_fitness[first] + m_fitness[second]);

            next_population.push_back(Individual::cross(m_individuals[pairs[i].first], m_individuals[pairs[i].second], first_advantage));
        }

        return next_population;
    }

    std::array<size_t, Population::Fitness_scale_array_size> Population::get_fitness_scale_array() const noexcept
    {
        std::array<size_t, Fitness_scale_array_size> fitness_scale_array{};

        size_t current_fitness_position = 0;
        float current_fitness = m_fitness[current_fitness_position];

        float delta = 1.f / static_cast<float>(Fitness_scale_array_size);
        float current_position = 0.f;

        for (size_t i = 0; i < Fitness_scale_array_size; ++i, current_position += delta)
        {
            while (current_position > current_fitness && current_fitness_position < m_size)
            {
                ++current_fitness_position;
                current_fitness += m_fitness[current_fitness_position];
            }

            fitness_scale_array[i] = current_fitness_position;
        }

        return fitness_scale_array;
    }
}