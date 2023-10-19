//
// Created by kango on 2023/01/04.
//

#include "Camera.h"
#include <iostream>

Camera::Camera(Eigen::Vector3d position, const Eigen::Vector3d &dir, const int &resolutionHeight, double aspectRatio, double verticalFoV, double focalLength)
    : org(std::move(position)), dir(dir.normalized()), focalLength(focalLength) {

    /// 度数法からradianに変換
    const auto theta = verticalFoV * EIGEN_PI / 180.0;

    /// thetaからフィルムの空間上における高さを計算
    const double filmHeight = 2.0 * tan(theta / 2.0) * focalLength;
    film = Film(resolutionHeight, aspectRatio, filmHeight);

    /// \カメラのローカル基底ベクトル
    /// カメラの方向ベクトルとy軸方向の単位ベクトルの外積で右向きのベクトルを計算\n
    /// 右向きのベクトルとカメラの方向ベクトルの外積で上向きのベクトルを計算\n
    right = this->dir.cross(Eigen::Vector3d::UnitY());
    up = right.cross(this->dir).normalized();
}

void Camera::filmView(const unsigned int &p_x, const unsigned int &p_y, Ray &out_ray) const {
    const auto pixelLocalPos = film.pixelLocalPosition(p_x, p_y);

    /// フィルムのピクセルのローカル座標をワールド座標に変換(カメラのorgを含む平面上に置く)
    const Eigen::Vector3d pixelWorldPos = org + film.filmSize.x() * right * (0.5 - pixelLocalPos.x()) + film.filmSize.y() * up * (pixelLocalPos.y() - 0.5) - focalLength * dir;

    /// レイの発射位置はフィルム上のピクセルの位置
    out_ray.org = org;

    /// 発射方向はカメラの原点と発射位置を結んだ点
    out_ray.dir = (org - pixelWorldPos).normalized();
}

const Film &Camera::getFilm() const {
    return film;
}

