#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <three-dim-util/gl-wrapper.hpp>
#include <Eigen/Core>

namespace threedimutil
{
    inline void draw_point(const Eigen::Vector3d& p)
    {
        assert(p.rows() == 3);
        glBegin(GL_POINTS);
        vertex_3d(p);
        glEnd();
    }
    
    inline void draw_points(const Eigen::MatrixXd& P)
    {
        assert(P.cols() == 3);
        glBegin(GL_POINTS);
        for (int i = 0; i < P.rows(); ++ i)
        {
            vertex_3d(P.row(i));
        }
        glEnd();
    }

    inline void draw_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C)
    {
        assert(P.cols() == 3);
        assert(C.cols() == 3);
        glBegin(GL_POINTS);
        for (int i = 0; i < P.rows(); ++ i)
        {
            color_3d(C.row(i));
            vertex_3d(P.row(i));
        }
        glEnd();
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
}

#endif /* draw_utils_hpp */
