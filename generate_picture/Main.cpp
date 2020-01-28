#include <SFML/Graphics.hpp>
#include <iostream>

#include "ImageGenerator.h"

#include "Individual.h"



int main()
{
    sf::RenderWindow window(sf::VideoMode(1700, 900), "Hello from SFML");

    unsigned int const width = 100;
    unsigned int const height = 100;

    ga_gp::Individual individual_1{};
    
    sf::Image const image_1 = ga_gp::generate_image(width, height, individual_1.get_points(width, height));

    sf::Texture texture_1{};

    texture_1.loadFromImage(image_1);

    sf::Sprite sprite_1{};
    sprite_1.setTexture(texture_1);

    sprite_1.setScale(8.f, 8.f);



    ga_gp::Individual individual_2{};

    sf::Image const image_2 = ga_gp::generate_image(width, height, individual_2.get_points(width, height));

    sf::Texture texture_2{};

    texture_2.loadFromImage(image_2);

    sf::Sprite sprite_2{};
    sprite_2.setTexture(texture_2);

    sprite_2.setScale(8.f, 8.f);
    sprite_2.setPosition(900.f, 0.f);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color{ 64, 64, 64 });

        window.draw(sprite_1);
        window.draw(sprite_2);

        window.display();
    }
    
    return 0;
}