#ifndef GL_WRAPPER_HPP
#define GL_WRAPPER_HPP

#include <OpenGL/gl.h>
#include <Eigen/Core>

namespace threedimutil
{
    inline void glColor(const Eigen::Vector3d& c) { glColor3d(c(0), c(1), c(2)); }
    inline void glColor(const Eigen::Vector3d& c, double a) { glColor4d(c(0), c(1), c(2), a); }
    inline void glVertex(const Eigen::Vector2d& v) { glVertex2d(v(0), v(1)); }
    inline void glVertex(const Eigen::Vector3d& v) { glVertex3d(v(0), v(1), v(2)); }
}

#endif // GL_WRAPPER_HPP
