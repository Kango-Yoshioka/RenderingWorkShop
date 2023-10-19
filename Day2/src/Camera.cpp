//
// Created by kango on 2023/01/04.
//

#include "Camera.h"
#include <iostream>

Camera::Camera(Eigen::Vector3d org, const Eigen::Vector3d &dir, const int &resolutionHeight, double aspectRatio, double verticalFoV, double focalLength)
    : m_org(std::move(org)), dir(dir.normalized()), m_focalLength(focalLength) {

    /// 度数法からradianに変換
    const auto theta = verticalFoV * EIGEN_PI / 180.0;

    /// thetaからフィルムの空間上における高さを計算
    const double filmHeight = 2.0 * tan(theta / 2.0) * focalLength;
    m_film = Film(resolutionHeight, aspectRatio, filmHeight);

    /// \カメラのローカル基底ベクトル
    /// カメラの方向ベクトルとy軸方向の単位ベクトルの外積で右向きのベクトルを計算\n
    /// 右向きのベクトルとカメラの方向ベクトルの外積で上向きのベクトルを計算\n
    m_right = this->dir.cross(Eigen::Vector3d::UnitY());
    m_up = m_right.cross(this->dir).normalized();
}

void Camera::filmView(const unsigned int &p_x, const unsigned int &p_y, Ray &out_ray) const {
    const auto pixelLocalPos = m_film.pixelLocalPosition(p_x, p_y);

    /// フィルムのピクセルのローカル座標をワールド座標に変換(カメラのorgを含む平面上に置く)
    const Eigen::Vector3d pixelWorldPos = m_org + m_film.filmSize.x() * m_right * (0.5 - pixelLocalPos.x()) + m_film.filmSize.y() * m_up * (pixelLocalPos.y() - 0.5) - m_focalLength * dir;

    /// レイの発射位置はフィルム上のピクセルの位置
    out_ray.org = m_org;

    /// 発射方向はカメラの原点と発射位置を結んだ点
    out_ray.dir = (m_org - pixelWorldPos).normalized();
}

const Film &Camera::getFilm() const {
    return m_film;
}

