#ifndef GLUT_WRAPPER_HPP
#define GLUT_WRAPPER_HPP

#include <Eigen/Core>

namespace threedimutil
{
    void drawSphere(double r, const Eigen::Vector3d& p, int res = 60);
    void drawSphere(double r, int res = 60);
    void drawFrame(double length, double width = 3.0);
    void drawRectangle(const Eigen::Vector2d& translate, double x, double y);
    void drawRectangle(double trans_x, double trans_y, double size_x, double size_y);
    void drawEllipse(double r_x, double r_y, double trans_x, double trans_y);
}

#endif // GLUT_WRAPPER_HPP
