#include <three-dim-util/camera.hpp>
#include <Eigen/Geometry>

namespace threedimutil
{
    using namespace Eigen;
    
    Camera::Camera() :
    position_(- 2.0, 0.0, 3.0),
    target_(0.0, 0.0, 0.0),
    up_(0.0, 1.0, 0.0),
    mode_(Mode::None)
    {
    }
    
    void Camera::RotateAroundTarget(double theta_in_radian)
    {
        // Horizontal rotation
        const Matrix3d rot_h = AngleAxisd(theta_in_radian, up_).matrix();
        
        // Update the camera position
        position_ = target_ + rot_h * (position_ - target_);
    }
    
    void Camera::BeginTrackball(int x, int y, Mode mode)
    {
        mode_ = mode;
        prev_position_ = Vector2i(x, y);
    }
    
    void Camera::MoveTrackball(int x, int y)
    {
        constexpr double scale_x = 2000.0;
        constexpr double scale_y = 2000.0;
        
        switch (mode_)
        {
            case Mode::None:
                break;
            case Mode::Pan:
            {
                const Vector3d eye      = position_ - target_;
                const Vector3d left_dir = eye.cross(up_);
                const double   len      = eye.norm();
                const double   diff_x   = static_cast<double>(x - prev_position_(0)) / scale_x;
                const double   diff_y   = static_cast<double>(y - prev_position_(1)) / scale_y;
                const Vector3d trans_x  = len * diff_x * left_dir.normalized();
                const Vector3d trans_y  = len * diff_y * up_.normalized();
                position_ += trans_x;
                position_ += trans_y;
                target_   += trans_x;
                target_   += trans_y;
                break;
            }
            case Mode::Rotate:
            {
                const double theta_x = (2.0 * M_PI * static_cast<double>(x - prev_position_(0))) / scale_x;
                const double theta_y = (2.0 * M_PI * static_cast<double>(y - prev_position_(1))) / scale_y;
                
                // Horizontal rotation
                const Matrix3d rot_h = AngleAxisd(- theta_x, up_).matrix();
                Vector3d eye = position_ - target_;
                
                // Vertical rotation
                const Vector3d left_dir = eye.cross(up_).normalized();
                const Matrix3d rot_v    = AngleAxisd(theta_y, left_dir).matrix();
                
                const double test = left_dir.dot((rot_v * eye).cross(up_));
                if (test > 0.0) {
                    eye = rot_v * rot_h * eye;
                }
                
                position_ = target_ + eye;
                break;
            }
            case Mode::Zoom:
            {
                constexpr double scale = 0.5;
                
                const double   speed   = static_cast<double>(y - prev_position_(1)) / scale_y;
                const Vector3d eye_ray = speed * (target_ - position_);
                position_ += scale * eye_ray;
                break;
            }
            default:
                break;
        }
        
        prev_position_ = Vector2i(x, y);
    }
    
    void Camera::EndTrackball()
    {
        mode_ = Mode::None;
    }
}
