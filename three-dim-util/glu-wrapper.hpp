#ifndef glu_wrapper_hpp
#define glu_wrapper_hpp

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
}

#endif /* glu_wrapper_hpp */
