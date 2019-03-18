#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <three-dim-util/opengl2/gl.hpp>
#include <three-dim-util/opengl2/gl-wrappers.hpp>
#include <three-dim-util/opengl2/primitives/cube.hpp>
#include <three-dim-util/opengl2/primitives/sphere.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace threedimutil
{
    inline void draw_point_with_border(const Eigen::Vector3d& p, double size = 12.0)
    {
        GLint function;
        internal::GlFunctions::get()->glGetIntegerv(GL_DEPTH_FUNC, &function);
        
        GLdouble color[4];
        internal::GlFunctions::get()->glGetDoublev(GL_CURRENT_COLOR, color);
        
        internal::GlFunctions::get()->glDepthFunc(GL_LEQUAL);
        
        internal::GlFunctions::get()->glPointSize(size);
        internal::GlFunctions::get()->glColor3d(1.0, 1.0, 1.0);
        internal::GlFunctions::get()->glBegin(GL_POINTS);
        vertex_3d(p);
        internal::GlFunctions::get()->glEnd();
        
        glPointSize(size * 0.8);
        internal::GlFunctions::get()->glColor4dv(color);
        internal::GlFunctions::get()->glBegin(GL_POINTS);
        vertex_3d(p);
        internal::GlFunctions::get()->glEnd();
        
        internal::GlFunctions::get()->glDepthFunc(function);
    }
    
    inline void draw_point(const Eigen::Vector3d& p, double size = 12.0)
    {
        internal::GlFunctions::get()->glPointSize(size);
        internal::GlFunctions::get()->glBegin(GL_POINTS);
        vertex_3d(p);
        internal::GlFunctions::get()->glEnd();
    }
    
    inline void draw_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C, double size = 12.0)
    {
        assert(P.rows() == 3);
        assert(C.rows() == 3);
        assert(P.cols() == C.cols());
        
        const int num_points = static_cast<int>(P.cols());
        
        internal::GlFunctions::get()->glPointSize(size);
        internal::GlFunctions::get()->glEnableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glEnableClientState(GL_COLOR_ARRAY);
        internal::GlFunctions::get()->glVertexPointer(3, GL_DOUBLE, 0, P.data());
        internal::GlFunctions::get()->glColorPointer(3, GL_DOUBLE, 0, C.data());
        internal::GlFunctions::get()->glDrawArrays(GL_POINTS, 0, num_points);
        internal::GlFunctions::get()->glDisableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glDisableClientState(GL_COLOR_ARRAY);
    }
    
    inline void draw_edges(const Eigen::MatrixXd& P, const Eigen::MatrixXi& E)
    {
        assert(P.rows() == 3);
        assert(E.rows() == 2);
        internal::GlFunctions::get()->glBegin(GL_LINES);
        for (int i = 0; i < E.cols(); ++ i)
        {
            vertex_3d(P.col(E(0, i)));
            vertex_3d(P.col(E(1, i)));
        }
        internal::GlFunctions::get()->glEnd();
    }
    
    inline void draw_mesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F)
    {
        assert(V.rows() == 3);
        assert(F.rows() == 3);
        internal::GlFunctions::get()->glBegin(GL_TRIANGLES);
        for (int i = 0; i < F.cols(); ++ i)
        {
            vertex_3d(V.col(F(0, i)));
            vertex_3d(V.col(F(1, i)));
            vertex_3d(V.col(F(2, i)));
        }
        internal::GlFunctions::get()->glEnd();
    }
    
    inline void draw_cube(const Eigen::Vector3d& t, double x, double y, double z)
    {
        const GLboolean is_gl_normalize_enabled = internal::GlFunctions::get()->glIsEnabled(GL_NORMALIZE);
        internal::GlFunctions::get()->glEnable(GL_NORMALIZE);
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        internal::GlFunctions::get()->glPushMatrix();
        translate(t);
        internal::GlFunctions::get()->glScaled(x, y, z);
        Cube::Draw();
        internal::GlFunctions::get()->glPopMatrix();
        if (!is_gl_normalize_enabled) { internal::GlFunctions::get()->glDisable(GL_NORMALIZE); }
    }
    
    inline void draw_cube(double size = 1.0)
    {
        draw_cube(Eigen::Vector3d::Zero(), size, size, size);
    }
    
    inline void draw_circle(double radius, int resolution = 30)
    {
        Eigen::MatrixXd vertices(2, resolution);
        for (int i = 0; i < resolution; ++ i)
        {
            constexpr double pi = M_PI;
            const double theta = 2.0 * static_cast<double>(i) * pi / static_cast<double>(resolution);
            vertices.col(i) = radius * Eigen::Vector2d(std::cos(theta), std::sin(theta));
        }
        Eigen::MatrixXd normals = Eigen::MatrixXd::Zero(3, resolution);
        normals.row(2) = Eigen::VectorXd::Ones(resolution);
        
        internal::GlFunctions::get()->glEnableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glEnableClientState(GL_NORMAL_ARRAY);
        internal::GlFunctions::get()->glVertexPointer(2, GL_DOUBLE, 0, vertices.data());
        internal::GlFunctions::get()->glNormalPointer(GL_DOUBLE, 0, normals.data());
        internal::GlFunctions::get()->glDrawArrays(GL_TRIANGLE_FAN, 0, resolution);
        internal::GlFunctions::get()->glDisableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    inline void draw_cylinder(double radius, double height, int resolution = 30)
    {
        constexpr double pi = M_PI;
        
        static Eigen::MatrixXd vertices;
        static Eigen::MatrixXd normals;
        
        // Create the vertex data if this is the first draw call or the resolution is specified differently from the last time
        if (vertices.cols() != resolution * 4)
        {
            vertices.resize(3, resolution * 4);
            normals.resize(3, resolution * 4);
            
            for (int i = 0; i < resolution; ++ i)
            {
                const double theta_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(resolution);
                const double theta_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(resolution);
                const double cos_1   = std::cos(theta_1);
                const double cos_2   = std::cos(theta_2);
                const double sin_1   = std::sin(theta_1);
                const double sin_2   = std::sin(theta_2);
                
                vertices.col(i * 4 + 0) = Eigen::Vector3d(cos_1, sin_1, 0.0);
                vertices.col(i * 4 + 1) = Eigen::Vector3d(cos_2, sin_2, 0.0);
                vertices.col(i * 4 + 2) = Eigen::Vector3d(cos_2, sin_2, 1.0);
                vertices.col(i * 4 + 3) = Eigen::Vector3d(cos_1, sin_1, 1.0);
                
                normals.col(i * 4 + 0) = Eigen::Vector3d(cos_1, sin_1, 0.0);
                normals.col(i * 4 + 1) = Eigen::Vector3d(cos_2, sin_2, 0.0);
                normals.col(i * 4 + 2) = Eigen::Vector3d(cos_2, sin_2, 0.0);
                normals.col(i * 4 + 3) = Eigen::Vector3d(cos_1, sin_1, 0.0);
            }
        }
        
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        
        // Draw a cylinder
        const GLboolean is_gl_normalize_enabled = internal::GlFunctions::get()->glIsEnabled(GL_NORMALIZE);
        internal::GlFunctions::get()->glEnable(GL_NORMALIZE);
        internal::GlFunctions::get()->glPushMatrix();
        internal::GlFunctions::get()->glScaled(radius, radius, height);
        internal::GlFunctions::get()->glEnableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glEnableClientState(GL_NORMAL_ARRAY);
        internal::GlFunctions::get()->glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
        internal::GlFunctions::get()->glNormalPointer(GL_DOUBLE, 0, normals.data());
        internal::GlFunctions::get()->glDrawArrays(GL_QUADS, 0, resolution * 4);
        internal::GlFunctions::get()->glDisableClientState(GL_VERTEX_ARRAY);
        internal::GlFunctions::get()->glDisableClientState(GL_NORMAL_ARRAY);
        internal::GlFunctions::get()->glPopMatrix();
        if (!is_gl_normalize_enabled) { internal::GlFunctions::get()->glDisable(GL_NORMALIZE); }
        
        // Draw circles at both ends
        internal::GlFunctions::get()->glPushMatrix();
        internal::GlFunctions::get()->glScaled(-1.0, 1.0, -1.0);
        draw_circle(radius, resolution);
        internal::GlFunctions::get()->glScaled(-1.0, 1.0, -1.0);
        internal::GlFunctions::get()->glTranslated(0.0, 0.0, height);
        draw_circle(radius, resolution);
        internal::GlFunctions::get()->glPopMatrix();
    }
    
    inline void draw_cylinder(double radius, const Eigen::Vector3d& p_1, const Eigen::Vector3d& p_2, int resolution = 30)
    {
        const Eigen::Vector3d t   = p_2 - p_1;
        const Eigen::Vector3d t_0 = Eigen::Vector3d::UnitZ();
        const double          c   = t.dot(t_0) / t.norm();
        const double          q   = std::acos(c);
        const Eigen::Vector3d ax  = t_0.cross(t).normalized();
        const double          h   = t.norm();
        const Eigen::Matrix4d rot = [&]()
        {
            return std::isnan(q) ? Eigen::Matrix4d::Identity() : Eigen::Affine3d(Eigen::AngleAxisd(q, ax)).matrix();
        }();
        
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        internal::GlFunctions::get()->glPushMatrix();
        translate(p_1);
        mult_matrix(rot);
        draw_cylinder(radius, h, resolution);
        internal::GlFunctions::get()->glPopMatrix();
    }
    
    inline void draw_sphere(double radius)
    {
        const GLboolean is_gl_normalize_enabled = internal::GlFunctions::get()->glIsEnabled(GL_NORMALIZE);
        internal::GlFunctions::get()->glEnable(GL_NORMALIZE);
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        internal::GlFunctions::get()->glPushMatrix();
        internal::GlFunctions::get()->glScaled(radius, radius, radius);
        Sphere::Draw();
        internal::GlFunctions::get()->glPopMatrix();
        if (!is_gl_normalize_enabled) { internal::GlFunctions::get()->glDisable(GL_NORMALIZE); }
    }
    
    inline void draw_sphere(double radius, const Eigen::Vector3d& t)
    {
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        internal::GlFunctions::get()->glPushMatrix();
        translate(t);
        draw_sphere(radius);
        internal::GlFunctions::get()->glPopMatrix();
    }
    
    inline void draw_floor(double                 size       = 200.0,
                           int                    resolution = 20,
                           const Eigen::Vector3d& color_1    = Eigen::Vector3d(0.9, 0.9, 0.9),
                           const Eigen::Vector3d& color_2    = Eigen::Vector3d(0.8, 0.8, 0.8))
    {
        const GLboolean is_gl_lighting_enabled = internal::GlFunctions::get()->glIsEnabled(GL_LIGHTING);
        internal::GlFunctions::get()->glDisable(GL_LIGHTING);
        
        internal::GlFunctions::get()->glBegin(GL_QUADS);
        for (int i = 0; i < resolution; ++ i)
        {
            const double x_m = - 0.5 * size + (i + 0) * size / static_cast<double>(resolution);
            const double x_p = - 0.5 * size + (i + 1) * size / static_cast<double>(resolution);
            
            for (int j = 0; j < resolution; ++ j)
            {
                const double z_m = - 0.5 * size + (j + 0) * size / static_cast<double>(resolution);
                const double z_p = - 0.5 * size + (j + 1) * size / static_cast<double>(resolution);
                
                if ((i + j) % 2 == 0) { color_3d(color_1); } else { color_3d(color_2); }
                
                internal::GlFunctions::get()->glVertex3d(x_p, 0.0, z_p);
                internal::GlFunctions::get()->glVertex3d(x_m, 0.0, z_p);
                internal::GlFunctions::get()->glVertex3d(x_m, 0.0, z_m);
                internal::GlFunctions::get()->glVertex3d(x_p, 0.0, z_m);
            }
        }
        internal::GlFunctions::get()->glEnd();
        
        if (is_gl_lighting_enabled) { internal::GlFunctions::get()->glEnable(GL_LIGHTING); }
    }
    
    inline void draw_rectangle(const Eigen::Vector2d& t, double width, double height)
    {
        internal::GlFunctions::get()->glMatrixMode(GL_MODELVIEW);
        internal::GlFunctions::get()->glPushMatrix();
        translate(t);
        internal::GlFunctions::get()->glBegin(GL_QUADS);
        internal::GlFunctions::get()->glNormal3d(0.0, 0.0, 1.0);
        internal::GlFunctions::get()->glVertex2d(+ 0.5 * width, + 0.5 * height);
        internal::GlFunctions::get()->glVertex2d(- 0.5 * width, + 0.5 * height);
        internal::GlFunctions::get()->glVertex2d(- 0.5 * width, - 0.5 * height);
        internal::GlFunctions::get()->glVertex2d(+ 0.5 * width, - 0.5 * height);
        internal::GlFunctions::get()->glEnd();
        internal::GlFunctions::get()->glPopMatrix();
    }
}

#endif /* draw_utils_hpp */
