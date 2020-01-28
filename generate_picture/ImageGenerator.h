#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

#include "Constants.h"

namespace ga_gp
{
    sf::Image generate_image(unsigned int width, unsigned int height, std::array<std::vector<std::vector<uint8_t>>, Color_type_count> const & points) noexcept;

    //std::vector<std::vector<std::array<uint8_t, Color_type_count>>> generate_points();
}