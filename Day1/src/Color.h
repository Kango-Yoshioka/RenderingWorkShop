//
// Created by kango on 2022/09/21.
//

#ifndef REFACTORINGIVR_COLOR_H
#define REFACTORINGIVR_COLOR_H

#include <Eigen/Dense>

class Color : public Eigen::Vector3d {
public:
    Color();
    Color(const double &r, const double &g, const double &b);
    explicit Color(const Eigen::Vector3d &v);
    explicit Color(const std::string &colorCode);

    Color changeLuminance(const double &l_out);
    double getLuminance() const;
private:
    void validateColorValues() const;
};
#endif //REFACTORINGIVR_COLOR_H
