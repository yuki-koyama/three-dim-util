#ifndef matrix_hpp
#define matrix_hpp

#include <three-dim-util/camera.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace threedimutil
{
    inline Eigen::Matrix4d make_perspective(double fov,
                                            double aspect,
                                            double near,
                                            double far)
    {
        const double val = std::tan(0.5 * fov);

        Eigen::Matrix4d mat = Eigen::Matrix4d::Zero();
        mat(0, 0) = 1.0 / (aspect * val);
        mat(1, 1) = 1.0 / (val);
        mat(2, 2) = far / (near - far);
        mat(3, 2) = - 1.0;
        mat(2, 3) = - 2.0 * (far * near) / (far - near);

        return mat;
    }

    inline Eigen::Matrix4d make_perspective(double   fov,
                                            unsigned frame_width,
                                            unsigned frame_height,
                                            double   near,
                                            double   far)
    {
        const double aspect = static_cast<double>(frame_width) / static_cast<double>(frame_height);

        return make_perspective(fov, aspect, near, far);
    }

    inline Eigen::Matrix4d make_look_at(const Eigen::Vector3d& camera_position,
                                        const Eigen::Vector3d& target_position,
                                        const Eigen::Vector3d& up_direction)
    {
        Eigen::Matrix4d mat = Eigen::Matrix4d::Identity();

        const Eigen::Vector3d forward = (target_position - camera_position).normalized();
        const Eigen::Vector3d side    = forward.cross(up_direction).normalized();
        const Eigen::Vector3d up      = side.cross(forward);

        mat.block<1, 3>(0, 0) = side.transpose();
        mat.block<1, 3>(1, 0) = up.transpose();
        mat.block<1, 3>(2, 0) = - forward.transpose();
        mat(0, 3) = - side.dot(camera_position);
        mat(1, 3) = - up.dot(camera_position);
        mat(2, 3) = forward.dot(camera_position);

        return mat;
    }
    
    inline Eigen::Matrix4d make_look_at(const Camera& camera)
    {
        return make_look_at(camera.position(), camera.target(), camera.up());
    }
    
    inline Eigen::Matrix4d make_ortho(double left,
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
        return (Eigen::Matrix4d() <<
                2.0 / dx,      0.0,        0.0,  tx,
                     0.0, 2.0 / dy,        0.0,  ty,
                     0.0,      0.0, - 2.0 / dz,  tz,
                     0.0,      0.0,        0.0, 1.0
                ).finished();
    }

    inline Eigen::Matrix4d make_ortho_2d(double left,
                                         double right,
                                         double bottom,
                                         double top)
    {
        const double dx = right - left;
        const double dy = top - bottom;
        const double tx = - (right + left) / dx;
        const double ty = - (top + bottom) / dy;
        return (Eigen::Matrix4d() <<
                2.0 / dx,      0.0,   0.0,  tx,
                     0.0, 2.0 / dy,   0.0,  ty,
                     0.0,      0.0, - 1.0, 0.0,
                     0.0,      0.0,   0.0, 1.0
                ).finished();
    }
}

#endif /* matrix_hpp */
