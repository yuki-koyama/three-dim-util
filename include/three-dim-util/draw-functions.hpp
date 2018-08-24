#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <three-dim-util/gl-wrapper.hpp>
#include <Eigen/Core>

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
        
        glPushMatrix();
        glScaled(x, y, z);
        translate(t);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vertices);
        glDrawArrays(GL_QUADS, 0, 4 * 6);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
    
    inline void draw_cube(double size = 1.0)
    {
        draw_cube(Eigen::Vector3d::Zero(), size, size, size);
    }
}

#endif /* draw_utils_hpp */
