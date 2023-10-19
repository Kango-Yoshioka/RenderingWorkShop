//
// Created by kango on 2022/12/11.
//

#include <iostream>
#include "Color.h"

Color::Color(const double &r, const double &g, const double &b) : Eigen::Vector3d(r, g, b) {
    std::cout << "call Color()" << std::endl;
    validateColorValues();
}

Color::Color(const Eigen::Vector3d &v) : Eigen::Vector3d(v) {
    validateColorValues();
}

Color::Color(const std::string &colorCode) {
    const auto rgb = (uint32_t) strtol((const char *) &colorCode[1], nullptr, 16);

    const auto red = (rgb >> 16) & 0xFF;
    const auto green = (rgb >> 8) & 0xFF;
    const auto blue = (rgb >> 0) & 0xFF;

    x() = static_cast<double>(red) / 255;
    y() = static_cast<double>(green) / 255;
    z() = static_cast<double>(blue) / 255;
}

void Color::validateColorValues() const {
    std::cout << "call validateColorValues()" << std::endl;
    if (x() < 0.0f || y() < 0.0f || z() < 0.0f) {
        std::cerr << "Invalid color values. Values must be 0 or greater.:\t" << transpose() << std::endl;
        // Perform appropriate error handling
        exit(EXIT_FAILURE);
    }
}

double Color::getLuminance() const {
    return dot(Eigen::Vector3d(0.2126, 0.7152, 0.0722));
}

Color Color::changeLuminance(const double &l_out) {
    Color col = Color(array() * (l_out / getLuminance()));
    return col;
}
