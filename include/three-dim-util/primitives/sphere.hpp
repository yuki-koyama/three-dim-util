#ifndef sphere_hpp
#define sphere_hpp

#include <three-dim-util/primitives/abstract-primitive.hpp>
#include <Eigen/Core>

namespace threedimutil
{
    class Sphere : public AbstractPrimitive
    {
    public:
        static void Initialize(int latitude_resolution = 20, int longitude_resolution = 30)
        {
            Sphere::GetInstance().latitude_resolution()  = latitude_resolution;
            Sphere::GetInstance().longitude_resolution() = longitude_resolution;
            Sphere::GetInstance().InitializeInternal();
        }
        
        static void Draw()
        {
            Sphere::GetInstance().DrawInternal();
        }
        
        static Sphere& GetInstance()
        {
            static Sphere sphere;
            return sphere;
        }
        
        int latitude_resolution() const { return latitude_resolution_; }
        int& latitude_resolution() { return latitude_resolution_; }
        int longitude_resolution() const { return longitude_resolution_; }
        int& longitude_resolution() { return longitude_resolution_; }
        
    private:
        void CreateVertexData()
        {
            constexpr double pi = M_PI;
            
            vertices_.resize(3, latitude_resolution_ * longitude_resolution_ * 6);
            for (int i = 0; i < longitude_resolution_; ++ i)
            {
                const double theta_xy_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(longitude_resolution_);
                const double theta_xy_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(longitude_resolution_);
                const double x_1        = std::cos(theta_xy_1);
                const double x_2        = std::cos(theta_xy_2);
                const double y_1        = std::sin(theta_xy_1);
                const double y_2        = std::sin(theta_xy_2);
                
                for (int j = 0; j < latitude_resolution_; ++ j)
                {
                    const double theta_z_1 = static_cast<double>(j + 0) * pi / static_cast<double>(latitude_resolution_);
                    const double theta_z_2 = static_cast<double>(j + 1) * pi / static_cast<double>(latitude_resolution_);
                    const double cos_1 = std::cos(theta_z_1);
                    const double cos_2 = std::cos(theta_z_2);
                    const double sin_1 = std::sin(theta_z_1);
                    const double sin_2 = std::sin(theta_z_2);
                    
                    const int offset = i * latitude_resolution_ * 6 + j * 6;
                    
                    vertices_.col(offset + 0) = Eigen::Vector3d(sin_2 * x_1, sin_2 * y_1, cos_2);
                    vertices_.col(offset + 1) = Eigen::Vector3d(sin_2 * x_2, sin_2 * y_2, cos_2);
                    vertices_.col(offset + 2) = Eigen::Vector3d(sin_1 * x_2, sin_1 * y_2, cos_1);
                    vertices_.col(offset + 3) = Eigen::Vector3d(sin_2 * x_1, sin_2 * y_1, cos_2);
                    vertices_.col(offset + 4) = Eigen::Vector3d(sin_1 * x_2, sin_1 * y_2, cos_1);
                    vertices_.col(offset + 5) = Eigen::Vector3d(sin_1 * x_1, sin_1 * y_1, cos_1);
                }
            }
            normals_ = vertices_;
        }
        
        int latitude_resolution_  = 20;
        int longitude_resolution_ = 30;
    };
}

#endif /* sphere_hpp */
