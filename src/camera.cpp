#include <three-dim-util/camera.hpp>
#include <Eigen/Geometry>

namespace threedimutil
{
    using namespace Eigen;

    Camera::Camera() :
    m_position(- 2.0, 0.0, 3.0),
    m_target(0.0, 0.0, 0.0),
    m_up(0.0, 1.0, 0.0),
    m_mode(Mode::None)
    {
    }

    void Camera::RotateAroundTarget(double theta_in_radian)
    {
        // Horizontal rotation
        const Matrix3d rot_h = AngleAxisd(theta_in_radian, m_up).matrix();

        // Update the camera position
        m_position = m_target + rot_h * (m_position - m_target);
    }

    void Camera::BeginTrackball(int x, int y, Mode mode)
    {
        m_mode = mode;
        m_prev_position = Vector2i(x, y);
    }

    void Camera::MoveTrackball(int x, int y)
    {
        constexpr double scale_x = 2000.0;
        constexpr double scale_y = 2000.0;

        switch (m_mode)
        {
            case Mode::None:
                break;
            case Mode::Pan:
            {
                const Vector3d eye      = m_position - m_target;
                const Vector3d left_dir = eye.cross(m_up);
                const double   len      = eye.norm();
                const double   diff_x   = static_cast<double>(x - m_prev_position(0)) / scale_x;
                const double   diff_y   = static_cast<double>(y - m_prev_position(1)) / scale_y;
                const Vector3d trans_x  = len * diff_x * left_dir.normalized();
                const Vector3d trans_y  = len * diff_y * m_up.normalized();
                m_position += trans_x;
                m_position += trans_y;
                m_target   += trans_x;
                m_target   += trans_y;
                break;
            }
            case Mode::Rotate:
            {
                const double theta_x = (2.0 * M_PI * static_cast<double>(x - m_prev_position(0))) / scale_x;
                const double theta_y = (2.0 * M_PI * static_cast<double>(y - m_prev_position(1))) / scale_y;

                // Horizontal rotation
                const Matrix3d rot_h = AngleAxisd(- theta_x, m_up).matrix();
                Vector3d eye = m_position - m_target;

                // Vertical rotation
                const Vector3d left_dir = eye.cross(m_up).normalized();
                const Matrix3d rot_v    = AngleAxisd(theta_y, left_dir).matrix();

                const double test = left_dir.dot((rot_v * eye).cross(m_up));
                if (test > 0.0) {
                    eye = rot_v * rot_h * eye;
                }

                m_position = m_target + eye;
                break;
            }
            case Mode::Zoom:
            {
                constexpr double scale = 0.5;

                const double   speed   = static_cast<double>(y - m_prev_position(1)) / scale_y;
                const Vector3d eye_ray = speed * (m_target - m_position);
                m_position += scale * eye_ray;
                break;
            }
            default:
                break;
        }

        m_prev_position = Vector2i(x, y);
    }

    void Camera::EndTrackball()
    {
        m_mode = Mode::None;
    }
}

Eigen::Matrix4d threedimutil::Camera::GetLookAtMatrix() const
{
    Eigen::Matrix4d mat = Eigen::Matrix4d::Identity();

    const Eigen::Vector3d forward = (m_target - m_position).normalized();
    const Eigen::Vector3d side    = forward.cross(m_up).normalized();
    const Eigen::Vector3d up      = side.cross(forward);

    mat.block<1, 3>(0, 0) = side.transpose();
    mat.block<1, 3>(1, 0) = up.transpose();
    mat.block<1, 3>(2, 0) = - forward.transpose();
    mat(0, 3) = - side.dot(m_position);
    mat(1, 3) = - up.dot(m_position);
    mat(2, 3) = forward.dot(m_position);

    return mat;
}
