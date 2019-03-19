#ifndef material_hpp
#define material_hpp

#include <three-dim-util/opengl2/gl.hpp>
#include <array>
#include <Eigen/Core>

namespace threedimutil
{
    struct Material
    {
        constexpr Material() {}
        
        constexpr Material(double r, double g, double b) :
        diffuse({{ static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b), 1.0 }})
        {
        }
        
        Material(const Eigen::Vector3d& diffuse) :
        diffuse({{ static_cast<GLfloat>(diffuse(0)), static_cast<GLfloat>(diffuse(1)), static_cast<GLfloat>(diffuse(2)), 1.0 }})
        {
        }
        
        std::array<GLfloat, 4> ambient   = {{ 0.1, 0.1, 0.1, 1.0 }};
        std::array<GLfloat, 4> diffuse   = {{ 0.5, 0.5, 0.5, 1.0 }};
        std::array<GLfloat, 4> specular  = {{ 0.8, 0.8, 0.8, 1.0 }};
        GLfloat                shininess = 100.0;
        
        void set() const
        {
            gl()->glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient  .data());
            gl()->glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse  .data());
            gl()->glMaterialfv(GL_FRONT, GL_SPECULAR,  specular .data());
            gl()->glMaterialf (GL_FRONT, GL_SHININESS, shininess       );
        }
    };
}

#endif /* material_hpp */
