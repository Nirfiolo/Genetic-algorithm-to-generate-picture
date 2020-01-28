#include "ImageGenerator.h"


namespace ga_gp
{
    sf::Image generate_image(unsigned int width, unsigned int height, std::array<std::vector<std::vector<uint8_t>>, Color_type_count> const & points) noexcept
    {
        sf::Image image{};

        std::vector<sf::Uint8> pixels(width * height * (Color_type_count + 1), Max_color);

        for (size_t i = 0; i < height; ++i)
        {
            for (size_t j = 0; j < width; ++j)
            {
                for (size_t k = 0; k < Color_type_count; ++k)
                {
                    pixels[4 * (i * width + j) + k] = points[k][i][j];
                }
            }
        }

        image.create(width, height, pixels.data());

        return image;
    }
}