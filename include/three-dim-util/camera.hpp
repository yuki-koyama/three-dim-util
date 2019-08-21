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

        double& fov() { return m_fov; }
        double fov() const { return m_fov; }

        Eigen::Vector3d& position() { return m_position; }
        const Eigen::Vector3d& position() const { return m_position; }
        Eigen::Vector3d& target() { return m_target; }
        const Eigen::Vector3d& target() const { return m_target; }
        Eigen::Vector3d& up() { return m_up; }
        const Eigen::Vector3d& up() const { return m_up; }

        // Method for obtaining matrices
        Eigen::Matrix4d GetLookAtMatrix() const;

        // Method for animated visualization
        void RotateAroundTarget(double theta_in_radian);

        // Methods for cursor interaction
        void BeginTrackball(int x, int y, Mode mode);
        void MoveTrackball(int x, int y);
        void EndTrackball();

    private:
        double m_fov = M_PI * 45.0 / 180.0;

        Eigen::Vector3d m_position;
        Eigen::Vector3d m_target;
        Eigen::Vector3d m_up;

        Mode m_mode;
        Eigen::Vector2i m_prev_position;
    };
}

#endif // CAMERA_HPP
