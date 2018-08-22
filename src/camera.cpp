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
        const double scale_x = 2000.0;
        const double scale_y = 2000.0;
        
        switch (mode_)
        {
            case Mode::None:
                break;
            case Mode::Pan:
            {
                Vector3d eye      = position_ - target_;
                Vector3d left_dir = eye.cross(up_);
                double   len      = eye.norm();
                Vector3d trans_x  = left_dir.normalized();
                Vector3d trans_y  = up_.normalized();
                
                trans_x *= len * static_cast<double>(x - prev_position_(0)) / scale_x;
                trans_y *= len * static_cast<double>(y - prev_position_(1)) / scale_y;
                
                position_ += trans_x;
                position_ += trans_y;
                target_   += trans_x;
                target_   += trans_y;
                break;
            }
            case Mode::Rotate:
            {
                double theta_x = (2.0 * M_PI * static_cast<double>(x - prev_position_(0))) / scale_x;
                double theta_y = (2.0 * M_PI * static_cast<double>(y - prev_position_(1))) / scale_y;
                
                // Horizontal rotation
                Matrix3d rot_h = AngleAxisd(- theta_x, up_).matrix();
                Vector3d eye   = position_ - target_;
                
                // Vertical rotation
                Vector3d left_dir = eye.cross(up_).normalized();
                Matrix3d rot_v    = AngleAxisd(theta_y, left_dir).matrix();
                
                double test = left_dir.dot((rot_v * eye).cross(up_));
                if (test > 0.0) {
                    eye = rot_v * rot_h * eye;
                }
                
                position_ = target_ + eye;
                break;
            }
            case Mode::Zoom:
            {
                Vector3d eye_direction = target_ - position_;
                eye_direction *= static_cast<double>(y - prev_position_(1)) / scale_y;
                position_ += 0.5 * eye_direction;
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
