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

    float step = 1.f;

    size_t current_expression = 16;
    std::string const file_name{ "../Data/expression" + std::to_string(current_expression) + ".dat" };

    std::ifstream file_in{ file_name };

    ga_gp::Individual individual;

    file_in >> individual;

    sf::Image image_1{};
    sf::Texture texture_1{};
    sf::Sprite sprite_1{};

    auto update = [&individual, &image_1, &texture_1, &sprite_1, &step, &width, &height]()
    {
        image_1 = ga_gp::generate_image(width, height, individual.get_points(width, height, step));
        texture_1.loadFromImage(image_1);
        sprite_1.setTexture(texture_1);
    };
    
    auto save_picture = [&image_1, current_expression]() noexcept
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
                }
                else if (event.key.code == sf::Keyboard::Num2)
                {
                    step *= 2.f;
                    update();
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    save_picture();
                }
            }
        }
        window.clear(sf::Color{ 64, 64, 64 });

        window.draw(sprite_1);

        window.display();
    }
}