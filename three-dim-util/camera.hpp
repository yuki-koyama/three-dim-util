#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Eigen/Core>

namespace threedimutil
{
    class Camera
    {
    public:
        Camera();
        
        enum class Mode {
            Rotate,
            Pan,
            Zoom,
            None
        };
        
        double vertical_angle_of_view_ = 45.0;
        
        Eigen::Vector3d position_;
        Eigen::Vector3d target_;
        Eigen::Vector3d up_;
        
        void BeginTrackball(int x, int y, Mode mode);
        void MoveTrackball (int x, int y);
        void EndTrackball  ();
        
    private:
        Mode mode_;
        Eigen::Vector2i prev_position_;
    };
}

#endif // CAMERA_HPP
