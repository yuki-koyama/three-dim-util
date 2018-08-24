#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <three-dim-util/gl-wrapper.hpp>
#include <Eigen/Core>

namespace threedimutil
{
    inline void draw_point(const Eigen::Vector3d& p, double size = 12.0)
    {
        assert(p.rows() == 3);

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
    
    inline void draw_points(const Eigen::MatrixXd& P, double size = 12.0)
    {
        assert(P.cols() == 3);
        for (int i = 0; i < P.rows(); ++ i)
        {
            draw_point(P.row(i), size);
        }
    }

    inline void draw_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C, double size = 12.0)
    {
        assert(P.cols() == 3);
        assert(C.cols() == 3);
        for (int i = 0; i < P.rows(); ++ i)
        {
            color_3d(C.row(i));
            draw_point(P.row(i), size);
        }
    }
    
    inline void draw_edges(const Eigen::MatrixXd& P, const Eigen::MatrixXi& E)
    {
        assert(P.cols() == 3);
        assert(E.cols() == 2);
        glBegin(GL_LINES);
        for (int i = 0; i < E.rows(); ++ i)
        {
            vertex_3d(P.row(E(i, 0)));
            vertex_3d(P.row(E(i, 1)));
        }
        glEnd();
    }
    
    inline void draw_faces(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F)
    {
        assert(V.cols() == 3);
        assert(F.cols() == 3);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < F.rows(); ++ i)
        {
            vertex_3d(V.row(F(i, 0)));
            vertex_3d(V.row(F(i, 1)));
            vertex_3d(V.row(F(i, 2)));
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
}

#endif /* draw_utils_hpp */
