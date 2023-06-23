//
// Created by kango on 2023/01/02.
//

#ifndef CUDATEST_FILM_H
#define CUDATEST_FILM_H

#include "Eigen/Dense"
#include "Image.h"

struct Film : public Image {
    /// 空間上におけるフィルムのサイズ
    Eigen::Vector2d filmSize;

    Film() = default;

    /**
     *
     * @param resolutionHeight
     * @param aspectRatio
     * @param filmHeight
     */
     Film(const int &resolutionHeight, const double &aspectRatio, const double &filmHeight)
     : Image(resolutionHeight * aspectRatio, resolutionHeight),
     filmSize(Eigen::Vector2d{filmHeight * aspectRatio, filmHeight}) {}

     Eigen::Vector2d pixelLocalPosition(const unsigned int &x, const unsigned int &y) const {
         return Eigen::Vector2d{(x + 0.5) / resX, (y + 0.5) / resY};
     }
};


#endif //CUDATEST_FILM_H
