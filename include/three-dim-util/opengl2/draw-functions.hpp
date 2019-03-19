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
        gl()->glGetIntegerv(GL_DEPTH_FUNC, &function);
        
        GLdouble color[4];
        gl()->glGetDoublev(GL_CURRENT_COLOR, color);
        
        gl()->glDepthFunc(GL_LEQUAL);
        
        gl()->glPointSize(size);
        gl()->glColor3d(1.0, 1.0, 1.0);
        gl()->glBegin(GL_POINTS);
        vertex_3d(p);
        gl()->glEnd();
        
        glPointSize(size * 0.8);
        gl()->glColor4dv(color);
        gl()->glBegin(GL_POINTS);
        vertex_3d(p);
        gl()->glEnd();
        
        gl()->glDepthFunc(function);
    }
    
    inline void draw_point(const Eigen::Vector3d& p, double size = 12.0)
    {
        gl()->glPointSize(size);
        gl()->glBegin(GL_POINTS);
        vertex_3d(p);
        gl()->glEnd();
    }
    
    inline void draw_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C, double size = 12.0)
    {
        assert(P.rows() == 3);
        assert(C.rows() == 3);
        assert(P.cols() == C.cols());
        
        const int num_points = static_cast<int>(P.cols());
        
        gl()->glPointSize(size);
        gl()->glEnableClientState(GL_VERTEX_ARRAY);
        gl()->glEnableClientState(GL_COLOR_ARRAY);
        gl()->glVertexPointer(3, GL_DOUBLE, 0, P.data());
        gl()->glColorPointer(3, GL_DOUBLE, 0, C.data());
        gl()->glDrawArrays(GL_POINTS, 0, num_points);
        gl()->glDisableClientState(GL_VERTEX_ARRAY);
        gl()->glDisableClientState(GL_COLOR_ARRAY);
    }
    
    inline void draw_edges(const Eigen::MatrixXd& P, const Eigen::MatrixXi& E)
    {
        assert(P.rows() == 3);
        assert(E.rows() == 2);
        gl()->glBegin(GL_LINES);
        for (int i = 0; i < E.cols(); ++ i)
        {
            vertex_3d(P.col(E(0, i)));
            vertex_3d(P.col(E(1, i)));
        }
        gl()->glEnd();
    }
    
    inline void draw_mesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F)
    {
        assert(V.rows() == 3);
        assert(F.rows() == 3);
        gl()->glBegin(GL_TRIANGLES);
        for (int i = 0; i < F.cols(); ++ i)
        {
            vertex_3d(V.col(F(0, i)));
            vertex_3d(V.col(F(1, i)));
            vertex_3d(V.col(F(2, i)));
        }
        gl()->glEnd();
    }
    
    inline void draw_cube(const Eigen::Vector3d& t, double x, double y, double z)
    {
        const GLboolean is_gl_normalize_enabled = gl()->glIsEnabled(GL_NORMALIZE);
        gl()->glEnable(GL_NORMALIZE);
        gl()->glMatrixMode(GL_MODELVIEW);
        gl()->glPushMatrix();
        translate(t);
        gl()->glScaled(x, y, z);
        Cube::Draw();
        gl()->glPopMatrix();
        if (!is_gl_normalize_enabled) { gl()->glDisable(GL_NORMALIZE); }
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
        
        gl()->glEnableClientState(GL_VERTEX_ARRAY);
        gl()->glEnableClientState(GL_NORMAL_ARRAY);
        gl()->glVertexPointer(2, GL_DOUBLE, 0, vertices.data());
        gl()->glNormalPointer(GL_DOUBLE, 0, normals.data());
        gl()->glDrawArrays(GL_TRIANGLE_FAN, 0, resolution);
        gl()->glDisableClientState(GL_VERTEX_ARRAY);
        gl()->glDisableClientState(GL_NORMAL_ARRAY);
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
        
        gl()->glMatrixMode(GL_MODELVIEW);
        
        // Draw a cylinder
        const GLboolean is_gl_normalize_enabled = gl()->glIsEnabled(GL_NORMALIZE);
        gl()->glEnable(GL_NORMALIZE);
        gl()->glPushMatrix();
        gl()->glScaled(radius, radius, height);
        gl()->glEnableClientState(GL_VERTEX_ARRAY);
        gl()->glEnableClientState(GL_NORMAL_ARRAY);
        gl()->glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
        gl()->glNormalPointer(GL_DOUBLE, 0, normals.data());
        gl()->glDrawArrays(GL_QUADS, 0, resolution * 4);
        gl()->glDisableClientState(GL_VERTEX_ARRAY);
        gl()->glDisableClientState(GL_NORMAL_ARRAY);
        gl()->glPopMatrix();
        if (!is_gl_normalize_enabled) { gl()->glDisable(GL_NORMALIZE); }
        
        // Draw circles at both ends
        gl()->glPushMatrix();
        gl()->glScaled(-1.0, 1.0, -1.0);
        draw_circle(radius, resolution);
        gl()->glScaled(-1.0, 1.0, -1.0);
        gl()->glTranslated(0.0, 0.0, height);
        draw_circle(radius, resolution);
        gl()->glPopMatrix();
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
        
        gl()->glMatrixMode(GL_MODELVIEW);
        gl()->glPushMatrix();
        translate(p_1);
        mult_matrix(rot);
        draw_cylinder(radius, h, resolution);
        gl()->glPopMatrix();
    }
    
    inline void draw_sphere(double radius)
    {
        const GLboolean is_gl_normalize_enabled = gl()->glIsEnabled(GL_NORMALIZE);
        gl()->glEnable(GL_NORMALIZE);
        gl()->glMatrixMode(GL_MODELVIEW);
        gl()->glPushMatrix();
        gl()->glScaled(radius, radius, radius);
        Sphere::Draw();
        gl()->glPopMatrix();
        if (!is_gl_normalize_enabled) { gl()->glDisable(GL_NORMALIZE); }
    }
    
    inline void draw_sphere(double radius, const Eigen::Vector3d& t)
    {
        gl()->glMatrixMode(GL_MODELVIEW);
        gl()->glPushMatrix();
        translate(t);
        draw_sphere(radius);
        gl()->glPopMatrix();
    }
    
    inline void draw_floor(double                 size       = 200.0,
                           int                    resolution = 20,
                           const Eigen::Vector3d& color_1    = Eigen::Vector3d(0.9, 0.9, 0.9),
                           const Eigen::Vector3d& color_2    = Eigen::Vector3d(0.8, 0.8, 0.8))
    {
        const GLboolean is_gl_lighting_enabled = gl()->glIsEnabled(GL_LIGHTING);
        gl()->glDisable(GL_LIGHTING);
        
        gl()->glBegin(GL_QUADS);
        for (int i = 0; i < resolution; ++ i)
        {
            const double x_m = - 0.5 * size + (i + 0) * size / static_cast<double>(resolution);
            const double x_p = - 0.5 * size + (i + 1) * size / static_cast<double>(resolution);
            
            for (int j = 0; j < resolution; ++ j)
            {
                const double z_m = - 0.5 * size + (j + 0) * size / static_cast<double>(resolution);
                const double z_p = - 0.5 * size + (j + 1) * size / static_cast<double>(resolution);
                
                if ((i + j) % 2 == 0) { color_3d(color_1); } else { color_3d(color_2); }
                
                gl()->glVertex3d(x_p, 0.0, z_p);
                gl()->glVertex3d(x_m, 0.0, z_p);
                gl()->glVertex3d(x_m, 0.0, z_m);
                gl()->glVertex3d(x_p, 0.0, z_m);
            }
        }
        gl()->glEnd();
        
        if (is_gl_lighting_enabled) { gl()->glEnable(GL_LIGHTING); }
    }
    
    inline void draw_rectangle(const Eigen::Vector2d& t, double width, double height)
    {
        gl()->glMatrixMode(GL_MODELVIEW);
        gl()->glPushMatrix();
        translate(t);
        gl()->glBegin(GL_QUADS);
        gl()->glNormal3d(0.0, 0.0, 1.0);
        gl()->glVertex2d(+ 0.5 * width, + 0.5 * height);
        gl()->glVertex2d(- 0.5 * width, + 0.5 * height);
        gl()->glVertex2d(- 0.5 * width, - 0.5 * height);
        gl()->glVertex2d(+ 0.5 * width, - 0.5 * height);
        gl()->glEnd();
        gl()->glPopMatrix();
    }
}

#endif /* draw_utils_hpp */
