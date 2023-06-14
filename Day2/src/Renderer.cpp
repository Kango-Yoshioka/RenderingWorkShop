//
// Created by kango on 2023/04/03.
//

#include "Renderer.h"
#include <iostream>

Renderer::Renderer(const std::vector<Body> &bodies, Camera camera, Color bgColor)
        : bodies(bodies), camera(std::move(camera)), bgColor(std::move(bgColor)), engine(0), dist(0, 1) {
}

/// 乱数生成
double Renderer::rand() const {
    return dist(engine);
}

bool Renderer::hitScene(const Ray &ray, RayHit &hit) const {
    hit.t = DBL_MAX;
    hit.idx = -1;
    for(int i = 0; i < bodies.size(); i++) {
        RayHit _hit;
        if(bodies[i].hit(ray, _hit) && _hit.t < hit.t) {
            hit.t = _hit.t;
            hit.idx = i;
            hit.point = _hit.point;
            hit.normal = _hit.normal;
        }
    }
    return hit.idx != -1;
}

Image Renderer::render() const {
    Image image(camera.getFilm().resolution.x(), camera.getFilm().resolution.y());
    /// フィルム上のピクセル全てに向けてレイを飛ばす
    for(int p_y = 0; p_y < image.height; p_y++) {
        for(int p_x = 0; p_x < image.width; p_x++) {
            const int p_idx = p_y * image.width + p_x;
            Color color;
            Ray ray; RayHit hit;
            camera.filmView(p_x, p_y, ray);

            /// レイを飛ばし、Bodyに当たったらその色を格納する\n
            /// 当たらなければ、背景色を返す
            color = hitScene(ray, hit) ? bodies[hit.idx].material.color * hit.normal.z() : bgColor;
            image.pixels[p_idx] = color;
        }
    }

    return image;
}

Image Renderer::directIlluminationRender(const unsigned int &samples) const {
    Image image(camera.getFilm().resolution.x(), camera.getFilm().resolution.y());
    /// フィルム上のピクセル全てに向けてレイを飛ばす
    for(int p_y = 0; p_y < image.height; p_y++) {
        for(int p_x = 0; p_x < image.width; p_x++) {
            const int p_idx = p_y * image.width + p_x;
            Ray ray; RayHit hit;
            camera.filmView(p_x, p_y, ray);

            if(hitScene(ray, hit)) {
                Color color = Color::Zero();
                for(int i = 0; i < samples; ++i) {
                    /// 衝突点xから半球上のランダムな方向にレイを飛ばす
                    Ray _ray; RayHit _hit;
                    diffuseSample(hit.point, hit.normal, _ray);

                    /// もし飛ばしたレイが光源に当たったら輝度を加算\n
                    if(hitScene(_ray, _hit) && bodies[_hit.idx].isLight()) {
                        color += bodies[hit.idx].getKd().cwiseProduct(bodies[_hit.idx].getEmission());
                    }
                }
                /// 自己発光 + 反射光
                image.pixels[p_idx] = bodies[hit.idx].getEmission() + color / static_cast<double>(samples);
            } else {
                image.pixels[p_idx] = bgColor;
            }

        }
    }

    return image;
}

void Renderer::diffuseSample(const Eigen::Vector3d &incidentPoint, const Eigen::Vector3d &normal, Ray &out_Ray) const {
    const double phi = 2.0 * EIGEN_PI * rand();
    const double theta = asin(sqrt(rand()));

    /// normalの方向をy軸とした正規直交基底を作る
    Eigen::Vector3d u, v;
    computeLocalFrame(normal, u, v);

    const double _x = sin(theta) * cos(phi);
    const double _y = cos(theta);
    const double _z = sin(theta) * sin(phi);

    out_Ray.dir = _x * u + _y * normal + _z * v;
    out_Ray.org = incidentPoint;
}

void Renderer::computeLocalFrame(const Eigen::Vector3d &w, Eigen::Vector3d &u, Eigen::Vector3d &v) {
    if(fabs(w.x()) > 1e-3)
        u = Eigen::Vector3d::UnitY().cross(w).normalized();
    else
        u = Eigen::Vector3d::UnitX().cross(w).normalized();

    v = w.cross(u);
}
