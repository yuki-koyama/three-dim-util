#ifndef GLUT_WRAPPER_HPP
#define GLUT_WRAPPER_HPP

#include <Eigen/Core>
#include <three-dim-util/gl.hpp>

namespace threedimutil
{
    void look_at(const Eigen::Vector3d& camera_position,
                 const Eigen::Vector3d& target_position,
                 const Eigen::Vector3d& up_direction)
    {
        gluLookAt(camera_position(0),
                  camera_position(1),
                  camera_position(2),
                  target_position(0),
                  target_position(1),
                  target_position(2),
                  up_direction(0),
                  up_direction(1),
                  up_direction(2));
    }

    void drawSphere(double r, const Eigen::Vector3d& p, int res = 60);
    void drawSphere(double r, int res = 60);
    void drawCylinder(double r, double h, int res = 60, bool center = true);
    void drawCylinder(double r1, double r2, double h, int res = 60, bool center = true);
    void drawCylinder(double r, const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, int res = 60);
    void drawCube(double size, bool center = true);
    void drawCube(double x, double y, double z, bool center = true);
    void drawCube(const Eigen::Vector3d& translate, double x, double y, double z, bool center = true);
    void drawFrame(double length, double width = 3.0);
    void drawRectangle(const Eigen::Vector2d& translate, double x, double y);
    void drawRectangle(double trans_x, double trans_y, double size_x, double size_y);
    void drawCircle(double r);
    void drawCircle(double r, double trans_x, double trans_y);
    void drawEllipse(double r_x, double r_y, double trans_x, double trans_y);
}

#endif // GLUT_WRAPPER_HPP
