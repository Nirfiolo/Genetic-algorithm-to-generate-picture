#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "../generate_picture/ImageGenerator.h"

#include "../generate_picture/Individual.h"



int main()
{
    sf::RenderWindow window(sf::VideoMode(1800, 900), "Hello from SFML");

    unsigned int const width = 1600;
    unsigned int const height = 800;

    float step = 0.25f;

    size_t current_expression = 0;
    size_t max_expression = 0;

    auto get_last = [&current_expression, &max_expression]()
    {
        std::ifstream file_in{ "../Data/counter.dat" };
        file_in >> current_expression;
        --current_expression;
        max_expression = current_expression;
        file_in.close();
    };
    get_last();


    ga_gp::Individual individual{};

    auto load_image = [&current_expression, &individual, &step]()
    {
        std::string const file_name{ "../Data/expression" + std::to_string(current_expression) + ".dat" };

        std::ifstream file_in{ file_name };

        file_in >> individual;
        file_in.close();

        step = 0.25f;
    };
    load_image();

    sf::Image image_1{};
    sf::Texture texture_1{};
    sf::Sprite sprite_1{};

    auto update = [&individual, &image_1, &texture_1, &sprite_1, &step, width, height]()
    {
        image_1 = ga_gp::generate_image(width, height, individual.get_points(width, height, step));
        texture_1.loadFromImage(image_1);
        sprite_1.setTexture(texture_1);
    };
    
    auto save_picture = [&image_1, &current_expression]() noexcept
    {
        image_1.saveToFile("../Data/image" + std::to_string(current_expression) + ".png");
    };

    update();

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
                if (event.key.code == sf::Keyboard::Num1)
                {
                    step /= 2.f;
                    update();

                    std::cout << "Updated /= 2" << '\n';
                }
                else if (event.key.code == sf::Keyboard::Num2)
                {
                    step *= 2.f;
                    update();

                    std::cout << "Updated *= 2" << '\n';
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    save_picture();

                    std::cout << "Saved" << '\n';
                }
                else if (event.key.code == sf::Keyboard::L)
                {
                    get_last();
                    load_image();
                    update();

                    std::cout << "Last was load" << '\n';
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    if (current_expression > 1)
                    {
                        --current_expression;
                        load_image();
                        update();

                        std::cout << "Switched to left" << '\n';
                    }
                    else
                    {
                        std::cout << "Error: Cannot be switch to left" << '\n';
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (current_expression < max_expression)
                    {
                        ++current_expression;
                        load_image();
                        update();

                        std::cout << "Switched to right" << '\n';
                    }
                    else
                    {
                        std::cout << "Error: Cannot be switch to right" << '\n';
                    }
                }
            }
        }
        window.clear(sf::Color{ 64, 64, 64 });

        window.draw(sprite_1);

        window.display();
    }
}