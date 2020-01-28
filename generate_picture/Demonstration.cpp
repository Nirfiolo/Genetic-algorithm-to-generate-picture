#include "Demonstration.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ImageGenerator.h"

#include "Individual.h"
#include "Population.h"

#include <fstream>


namespace ga_gp
{
    void demonstration() noexcept
    {
        sf::RenderWindow window(sf::VideoMode(1700, 900), "Hello from SFML");

        unsigned int const width = 200;
        unsigned int const height = 200;

        sf::Image image_1{};
        sf::Texture texture_1{};
        sf::Sprite sprite_1{};

        sprite_1.setScale(4.f, 4.f);

        sf::Image image_2{};
        sf::Texture texture_2{};
        sf::Sprite sprite_2{};

        sprite_2.setScale(4.f, 4.f);
        sprite_2.setPosition(900.f, 0.f);



        size_t const population_size{ 15 };

        ga_gp::Population population{ population_size };

        std::vector<sf::Image> images(population_size);
        std::vector<sf::Texture> textures(population_size);
        std::vector<sf::Sprite> sprites(population_size);

        float const scale = 1.f;

        std::vector<sf::Vector2f> positions(population_size);
        float const between = 300.f;
        size_t const rows = 3;
        size_t const cols = 5;

        size_t current = 0;
        for (size_t i = 0; i < cols; ++i)
        {
            for (size_t j = 0; j < rows; ++j)
            {
                positions[current].x = between * static_cast<float>(i);
                positions[current].y = between * static_cast<float>(j);
                ++current;
            }
        }

        auto update_two_pictures = [&population, &image_1, &image_2, &texture_1,  &texture_2, &sprite_1, &sprite_2, &scale, &width, &height]() noexcept
        {
            std::pair<Individual, Individual> const & individuals = population.get_pair();

            image_1 = generate_image(width, height, individuals.first.get_points(width, height));
            texture_1.loadFromImage(image_1);
            sprite_1.setTexture(texture_1);

            image_2 = generate_image(width, height, individuals.second.get_points(width, height));
            texture_2.loadFromImage(image_2);
            sprite_2.setTexture(texture_2);
            
        };

        auto update_picture_list = [&population, &images, &textures, &sprites, &scale, &positions, &population_size, &width, &height]() noexcept
        {
            std::vector<Individual> const & individuals = population.get_individuals();

            for (size_t i = 0; i < population_size; ++i)
            {
                images[i] = generate_image(width, height, individuals[i].get_points(width, height));
                textures[i].loadFromImage(images[i]);
                sprites[i].setTexture(textures[i]);
                sprites[i].setScale(scale, scale);
                sprites[i].setPosition(positions[i]);
            }
        };

        size_t current_image = 0;

        {
            std::ifstream file_in{ "../Data/counter.dat" };
            file_in >> current_image;
            file_in.close();
        }


        auto save_picture = [&current_image](sf::Image const & image) noexcept
        {
            image.saveToFile("../Data/image" + std::to_string(current_image) + ".png");
        };

        auto save_expression = [&current_image, &population](bool is_first) noexcept
        {
            std::ofstream file_out{ "../Data/expression" + std::to_string(current_image) + ".dat" };
            ++current_image;

            std::pair<Individual, Individual> const & individuals = population.get_pair();

            if (is_first)
            {
                file_out << individuals.first;
            }
            else
            {
                file_out << individuals.second;
            }

            file_out.close();

            file_out.open("../Data/counter.dat");
            file_out << current_image;

            file_out.close();
        };


        update_two_pictures();
        update_picture_list();

        Mode mode = Mode::ShowAll;

        while (window.isOpen())
        {
            sf::Event event;
            if (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::F1)
                    {
                        mode = Mode::ShowAll;
                    }
                    else if (event.key.code == sf::Keyboard::F2)
                    {
                        mode = Mode::ChooseOne;
                        population.reroll_pair();
                        update_two_pictures();
                    }
                    else if (event.key.code == sf::Keyboard::N && mode == Mode::ShowAll)
                    {
                        population.go_to_next_evolution();
                        update_picture_list();
                    }
                    else if (event.key.code == sf::Keyboard::Tab && mode == Mode::ChooseOne)
                    {
                        population.reroll_pair();
                        update_two_pictures();
                    }
                    else if (event.key.code == sf::Keyboard::Num1 && mode == Mode::ChooseOne)
                    {
                        population.submit_result(true);
                        population.reroll_pair();
                        update_two_pictures();
                    }
                    else if (event.key.code == sf::Keyboard::Num2 && mode == Mode::ChooseOne)
                    {
                        population.submit_result(false);
                        population.reroll_pair();
                        update_two_pictures();
                    }
                    else if (event.key.code == sf::Keyboard::Num9 && mode == Mode::ChooseOne)
                    {
                        save_picture(image_1);
                        save_expression(true);
                    }
                    else if (event.key.code == sf::Keyboard::Num0 && mode == Mode::ChooseOne)
                    {
                        save_picture(image_2);
                        save_expression(false);
                    }
                }
            }
            window.clear(sf::Color{ 64, 64, 64 });

            if (mode == Mode::ShowAll)
            {
                for (size_t i = 0; i < population_size; ++i)
                {
                    window.draw(sprites[i]);
                }
            }
            else if (mode == Mode::ChooseOne)
            {
                window.draw(sprite_1);
                window.draw(sprite_2);
            }
            


            window.display();
        }
    }
}