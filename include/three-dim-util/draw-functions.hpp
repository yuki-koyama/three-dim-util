#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <three-dim-util/gl-wrapper.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace threedimutil
{
    inline void draw_point_with_border(const Eigen::Vector3d& p, double size = 12.0)
    {
        GLint function;
        glGetIntegerv(GL_DEPTH_FUNC, &function);
        
        GLdouble color[4];
        glGetDoublev(GL_CURRENT_COLOR, color);
        
        glDepthFunc(GL_LEQUAL);
        
        glPointSize(size);
        glColor3d(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        vertex_3d(p);
        glEnd();
        
        glPointSize(size * 0.8);
        glColor4dv(color);
        glBegin(GL_POINTS);
        vertex_3d(p);
        glEnd();
        
        glDepthFunc(function);
    }
    
    inline void draw_point(const Eigen::Vector3d& p, double size = 12.0)
    {
        glPointSize(size);
        glBegin(GL_POINTS);
        vertex_3d(p);
        glEnd();
    }
    
    inline void draw_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C, double size = 12.0)
    {
        assert(P.rows() == 3);
        assert(C.rows() == 3);
        assert(P.cols() == C.cols());
        
        const int num_points = P.cols();
        
        glPointSize(size);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, P.data());
        glColorPointer(3, GL_DOUBLE, 0, C.data());
        glDrawArrays(GL_POINTS, 0, num_points);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
    
    inline void draw_edges(const Eigen::MatrixXd& P, const Eigen::MatrixXi& E)
    {
        assert(P.rows() == 3);
        assert(E.rows() == 2);
        glBegin(GL_LINES);
        for (int i = 0; i < E.cols(); ++ i)
        {
            vertex_3d(P.col(E(0, i)));
            vertex_3d(P.col(E(1, i)));
        }
        glEnd();
    }
    
    inline void draw_mesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F)
    {
        assert(V.rows() == 3);
        assert(F.rows() == 3);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < F.cols(); ++ i)
        {
            vertex_3d(V.col(F(0, i)));
            vertex_3d(V.col(F(1, i)));
            vertex_3d(V.col(F(2, i)));
        }
        glEnd();
    }
    
    inline void draw_cube(const Eigen::Vector3d& t, double x, double y, double z)
    {
        constexpr GLdouble vertices[] =
        {
            +0.5, +0.5, +0.5,
            -0.5, +0.5, +0.5,
            -0.5, -0.5, +0.5,
            +0.5, -0.5, +0.5,
            
            +0.5, +0.5, -0.5,
            +0.5, +0.5, +0.5,
            +0.5, -0.5, +0.5,
            +0.5, -0.5, -0.5,
            
            -0.5, +0.5, -0.5,
            +0.5, +0.5, -0.5,
            +0.5, -0.5, -0.5,
            -0.5, -0.5, -0.5,
            
            -0.5, +0.5, +0.5,
            -0.5, +0.5, -0.5,
            -0.5, -0.5, -0.5,
            -0.5, -0.5, +0.5,
            
            +0.5, +0.5, -0.5,
            -0.5, +0.5, -0.5,
            -0.5, +0.5, +0.5,
            +0.5, +0.5, +0.5,
            
            -0.5, -0.5, +0.5,
            -0.5, -0.5, -0.5,
            +0.5, -0.5, -0.5,
            +0.5, -0.5, +0.5
        };
        
        constexpr GLdouble normals[] =
        {
            0.0, 0.0, +1.0,
            0.0, 0.0, +1.0,
            0.0, 0.0, +1.0,
            0.0, 0.0, +1.0,
            
            +1.0, 0.0, 0.0,
            +1.0, 0.0, 0.0,
            +1.0, 0.0, 0.0,
            +1.0, 0.0, 0.0,
            
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            
            0.0, +1.0, 0.0,
            0.0, +1.0, 0.0,
            0.0, +1.0, 0.0,
            0.0, +1.0, 0.0,
            
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0
        };
        
        const GLboolean is_gl_normalize_enabled = glIsEnabled(GL_NORMALIZE);
        
        glEnable(GL_NORMALIZE);
        glPushMatrix();
        glScaled(x, y, z);
        translate(t);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vertices);
        glNormalPointer(GL_DOUBLE, 0, normals);
        glDrawArrays(GL_QUADS, 0, 4 * 6);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glPopMatrix();
        
        if (!is_gl_normalize_enabled) { glDisable(GL_NORMALIZE); }
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
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertices.data());
        glNormalPointer(GL_DOUBLE, 0, normals.data());
        glDrawArrays(GL_TRIANGLE_FAN, 0, resolution);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    inline void draw_cylinder(double radius, double height, int resolution = 30)
    {
        constexpr double pi = M_PI;
        
        // Draw a cylinder
        Eigen::MatrixXd vertices(3, resolution * 4);
        Eigen::MatrixXd normals(3, resolution * 4);
        for (int i = 0; i < resolution; ++ i)
        {
            const double theta_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(resolution);
            const double theta_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(resolution);
            const double cos_1   = std::cos(theta_1);
            const double cos_2   = std::cos(theta_2);
            const double sin_1   = std::sin(theta_1);
            const double sin_2   = std::sin(theta_2);
            
            vertices.col(i * 4 + 0) = Eigen::Vector3d(radius * cos_1, radius * sin_1, 0.0);
            vertices.col(i * 4 + 1) = Eigen::Vector3d(radius * cos_2, radius * sin_2, 0.0);
            vertices.col(i * 4 + 2) = Eigen::Vector3d(radius * cos_2, radius * sin_2, height);
            vertices.col(i * 4 + 3) = Eigen::Vector3d(radius * cos_1, radius * sin_1, height);
            
            normals.col(i * 4 + 0) = Eigen::Vector3d(cos_1, sin_1, 0.0);
            normals.col(i * 4 + 1) = Eigen::Vector3d(cos_2, sin_2, 0.0);
            normals.col(i * 4 + 2) = Eigen::Vector3d(cos_2, sin_2, 0.0);
            normals.col(i * 4 + 3) = Eigen::Vector3d(cos_1, sin_1, 0.0);
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
        glNormalPointer(GL_DOUBLE, 0, normals.data());
        glDrawArrays(GL_QUADS, 0, resolution * 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        
        // Draw circles at both ends
        glPushMatrix();
        glScaled(-1.0, 1.0, -1.0);
        draw_circle(radius, resolution);
        glScaled(-1.0, 1.0, -1.0);
        glTranslated(0.0, 0.0, height);
        draw_circle(radius, resolution);
        glPopMatrix();
    }
    
    inline void draw_cylinder(double radius, const Eigen::Vector3d& p_1, const Eigen::Vector3d& p_2, int resolution = 30)
    {
        const Eigen::Vector3d t   = p_2 - p_1;
        const Eigen::Vector3d t_0 = Eigen::Vector3d(0.0, 0.0, 1.0);
        const double          c   = t.dot(t_0) / t.norm();
        const double          q   = std::acos(c);
        const Eigen::Vector3d ax  = t_0.cross(t).normalized();
        const double          h   = t.norm();
        const Eigen::Matrix4d rot = [&]()
        {
            return std::isnan(q) ? Eigen::Matrix4d::Identity() : Eigen::Affine3d(Eigen::AngleAxisd(q, ax)).matrix();
        }();
        
        glPushMatrix();
        translate(p_1);
        mult_matrix(rot);
        draw_cylinder(radius, h, resolution);
        glPopMatrix();
    }
    
    inline void draw_sphere(double radius, int latitude_resolution = 10, int longitude_resolution = 20)
    {
        constexpr double pi = M_PI;
        
        Eigen::MatrixXd vertices(3, latitude_resolution * longitude_resolution * 4);
        Eigen::MatrixXd normals(3, latitude_resolution * longitude_resolution * 4);
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
                
                vertices.col(i * latitude_resolution * 4 + j * 4 + 0) = Eigen::Vector3d(sin_2 * x_1, sin_2 * y_1, cos_2);
                vertices.col(i * latitude_resolution * 4 + j * 4 + 1) = Eigen::Vector3d(sin_2 * x_2, sin_2 * y_2, cos_2);
                vertices.col(i * latitude_resolution * 4 + j * 4 + 2) = Eigen::Vector3d(sin_1 * x_2, sin_1 * y_2, cos_1);
                vertices.col(i * latitude_resolution * 4 + j * 4 + 3) = Eigen::Vector3d(sin_1 * x_1, sin_1 * y_1, cos_1);
            }
        }
        normals  = vertices;
        vertices = radius * vertices;
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
        glNormalPointer(GL_DOUBLE, 0, normals.data());
        glDrawArrays(GL_QUADS, 0, latitude_resolution * longitude_resolution * 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    inline void draw_sphere(double radius, const Eigen::Vector3d& t, int latitude_resolution = 10, int longitude_resolution = 20)
    {
        glPushMatrix();
        translate(t);
        draw_sphere(radius, latitude_resolution, longitude_resolution);
        glPopMatrix();
    }
    
    inline void draw_rectangle(const Eigen::Vector2d& t, double width, double height)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        translate(t);
        glBegin(GL_QUADS);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex2d(+ 0.5 * width, + 0.5 * height);
        glVertex2d(- 0.5 * width, + 0.5 * height);
        glVertex2d(- 0.5 * width, - 0.5 * height);
        glVertex2d(+ 0.5 * width, - 0.5 * height);
        glEnd();
        glPopMatrix();
    }
}

#endif /* draw_utils_hpp */
