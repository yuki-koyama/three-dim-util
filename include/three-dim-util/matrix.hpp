#ifndef matrix_hpp
#define matrix_hpp

#include <three-dim-util/camera.hpp>
#include <Eigen/Core>
#include <glm/gtc/matrix_transform.hpp>

namespace threedimutil
{
    inline Eigen::Matrix4d convert_matrix_type(const glm::mat4& mat)
    {
        return (Eigen::Matrix4d() <<
                mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                mat[0][3], mat[1][3], mat[2][3], mat[3][3]
                ).finished();
    }
    
    inline Eigen::Matrix4d make_perspective(double fov,
                                            double aspect,
                                            double near,
                                            double far)
    {
        const glm::mat4 mat = glm::perspective(fov, aspect, near, far);
        return convert_matrix_type(mat);
    }
    
    inline Eigen::Matrix4d make_look_at(const Eigen::Vector3d& camera_position,
                                        const Eigen::Vector3d& target_position,
                                        const Eigen::Vector3d& up_direction)
    {
        const glm::mat4 mat = glm::lookAt(glm::vec3(camera_position(0), camera_position(1), camera_position(2)),
                                          glm::vec3(target_position(0), target_position(1), target_position(2)),
                                          glm::vec3(up_direction(0), up_direction(1), up_direction(2)));
        return convert_matrix_type(mat);
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
