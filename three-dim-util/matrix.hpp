#ifndef matrix_hpp
#define matrix_hpp

#include <Eigen/Core>

namespace threedimutil
{
    inline Eigen::Matrix4d makeOrtho(double left,
                                     double right,
                                     double bottom,
                                     double top,
                                     double near,
                                     double far)
    {
        const double dx = right - left;
        const double dy = top - bottom;
        const double dz = far - near;
        const double tx = - (right + left) / dx;
        const double ty = - (top + bottom) / dy;
        const double tz = - (far + near) / dz;
        return (Eigen::Matrix4d() << 2.0 / dx,      0.0,        0.0,  tx,
                                          0.0, 2.0 / dy,        0.0,  ty,
                                          0.0,      0.0, - 2.0 / dz,  tz,
                                          0.0,      0.0,        0.0, 1.0).finished();
    }
    
    inline Eigen::Matrix4d makeOrtho2d(double left,
                                       double right,
                                       double bottom,
                                       double top)
    {
        const double dx = right - left;
        const double dy = top - bottom;
        const double tx = - (right + left) / dx;
        const double ty = - (top + bottom) / dy;
        return (Eigen::Matrix4d() << 2.0 / dx,      0.0,   0.0,  tx,
                                          0.0, 2.0 / dy,   0.0,  ty,
                                          0.0,      0.0, - 1.0, 0.0,
                                          0.0,      0.0,   0.0, 1.0).finished();
    }
}

#endif /* matrix_hpp */
