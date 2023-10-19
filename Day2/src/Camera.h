//
// Created by kango on 2023/01/02.
//

#ifndef CUDATEST_CAMERA_CUH
#define CUDATEST_CAMERA_CUH


#include "Ray.h"
#include "Film.h"

class Camera{
private:
    double m_focalLength;
    Eigen::Vector3d m_org, m_right, m_up;
    Film m_film;

public:
    /**
     * @param org カメラの位置
     * @param dir カメラの向き
     * @param resolutionHeight カメラの解像度（高さ）
     * @param aspectRatio アスペクト比
     * @param verticalFoV 視野角（高さ）
     * @param focalLength カメラとフィルムの距離
     */
    Camera(Eigen::Vector3d org, const Eigen::Vector3d &dir, const int &resolutionHeight, double aspectRatio, double verticalFoV, double focalLength=1.0);
    void filmView(const unsigned int &p_x, const unsigned int &p_y, Ray &out_ray) const;

    const Film &getFilm() const;

    Eigen::Vector3d dir;
};


#endif //CUDATEST_CAMERA_CUH
