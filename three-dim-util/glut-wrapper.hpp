#ifndef GLUT_WRAPPER_HPP
#define GLUT_WRAPPER_HPP

#include <string>
#include <vector>
#include <OpenGL/gl.h>
#include <Eigen/Core>

namespace threedimutil
{
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

    inline void glColor(const Eigen::Vector3d& c) { glColor3d(c(0), c(1), c(2)); }
    inline void glColor(const Eigen::Vector3d& c, double a) { glColor4d(c(0), c(1), c(2), a); }
    inline void glVertex(const Eigen::Vector2d& v) { glVertex2d(v(0), v(1)); }
}

#endif // GLUT_WRAPPER_HPP
