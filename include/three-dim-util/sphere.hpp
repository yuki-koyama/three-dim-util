#ifndef sphere_hpp
#define sphere_hpp

#include <three-dim-util/gl.hpp>
#include <iostream>
#include <Eigen/Core>

namespace threedimutil
{
    class Sphere
    {
    public:
        static void Initialize(int latitude_resolution = 20, int longitude_resolution = 30)
        {
            Sphere::GetInstance().InitializeInternal(latitude_resolution, longitude_resolution);
        }
        
        static void Draw()
        {
            Sphere::GetInstance().DrawInternal();
        }
        
    protected:
        static Sphere& GetInstance()
        {
            static Sphere sphere;
            return sphere;
        }
        
        void InitializeInternal(int latitude_resolution = 20, int longitude_resolution = 30)
        {
            constexpr double pi = M_PI;
            
            if (ready_)
            {
                std::cerr << "Warning: Initialize() is called but it is already initialized. This call is ignored." << std::endl;
                return;
            }
            
            vertices_.resize(3, latitude_resolution * longitude_resolution * 6);
            for (int i = 0; i < longitude_resolution; ++ i)
            {
                const double theta_xy_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(longitude_resolution);
                const double theta_xy_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(longitude_resolution);
                const double x_1        = std::cos(theta_xy_1);
                const double x_2        = std::cos(theta_xy_2);
                const double y_1        = std::sin(theta_xy_1);
                const double y_2        = std::sin(theta_xy_2);
                
                for (int j = 0; j < latitude_resolution; ++ j)
                {
                    const double theta_z_1 = static_cast<double>(j + 0) * pi / static_cast<double>(latitude_resolution);
                    const double theta_z_2 = static_cast<double>(j + 1) * pi / static_cast<double>(latitude_resolution);
                    const double cos_1 = std::cos(theta_z_1);
                    const double cos_2 = std::cos(theta_z_2);
                    const double sin_1 = std::sin(theta_z_1);
                    const double sin_2 = std::sin(theta_z_2);
                    
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 0) = Eigen::Vector3d(sin_2 * x_1, sin_2 * y_1, cos_2);
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 1) = Eigen::Vector3d(sin_2 * x_2, sin_2 * y_2, cos_2);
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 2) = Eigen::Vector3d(sin_1 * x_2, sin_1 * y_2, cos_1);
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 3) = Eigen::Vector3d(sin_2 * x_1, sin_2 * y_1, cos_2);
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 4) = Eigen::Vector3d(sin_1 * x_2, sin_1 * y_2, cos_1);
                    vertices_.col(i * latitude_resolution * 6 + j * 6 + 5) = Eigen::Vector3d(sin_1 * x_1, sin_1 * y_1, cos_1);
                }
            }
            normals_ = vertices_;
            
            glGenBuffers(1, &vertex_vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &normal_vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
            
            ready_ = true;
        }
        
        void DrawInternal()
        {
            if (!ready_) { Initialize(); }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            
            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            glVertexPointer(3, GL_DOUBLE, 0, NULL);
            
            glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            glNormalPointer(GL_DOUBLE, 0, NULL);
            
            glDrawArrays(GL_TRIANGLES, 0, vertices_.cols());
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
        }
        
    private:
        Sphere() = default;
        ~Sphere()
        {
            if (ready_)
            {
                glDeleteBuffers(1, &vertex_vbo_);
                glDeleteBuffers(1, &normal_vbo_);
            }
        }
        Sphere(const Sphere&)            = delete;
        Sphere& operator=(const Sphere&) = delete;
        Sphere(Sphere&&)                 = delete;
        Sphere& operator=(Sphere&&)      = delete;
        
        bool ready_ = false;
        
        GLuint vertex_vbo_;
        GLuint normal_vbo_;
        
        Eigen::MatrixXd vertices_;
        Eigen::MatrixXd normals_;
    };
}

#endif /* sphere_hpp */
