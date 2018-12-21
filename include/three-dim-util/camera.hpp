#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include <Eigen/Core>

namespace threedimutil
{
    class Camera
    {
    public:
        Camera();
        
        enum class Mode
        {
            Rotate,
            Pan,
            Zoom,
            None
        };
        
        double& vertical_angle_of_view() { return vertical_angle_of_view_; }
        double vertical_angle_of_view() const { return vertical_angle_of_view_; }
        
        Eigen::Vector3d& position() { return position_; }
        const Eigen::Vector3d& position() const { return position_; }
        Eigen::Vector3d& target() { return target_; }
        const Eigen::Vector3d& target() const { return target_; }
        Eigen::Vector3d& up() { return up_; }
        const Eigen::Vector3d& up() const { return up_; }
        
        // Method for animated visualization
        void RotateAroundTarget(double theta_in_radian);
        
        // Methods for cursor interaction
        void BeginTrackball(int x, int y, Mode mode);
        void MoveTrackball (int x, int y);
        void EndTrackball  ();
        
    private:
        double vertical_angle_of_view_ = M_PI * 45.0 / 180.0;
        
        Eigen::Vector3d position_;
        Eigen::Vector3d target_;
        Eigen::Vector3d up_;
        
        Mode mode_;
        Eigen::Vector2i prev_position_;
    };
}

#endif // CAMERA_HPP
