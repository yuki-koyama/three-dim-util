#ifndef draw_utils_hpp
#define draw_utils_hpp

#include <Eigen/Core>
#include <three-dim-util/gl-wrapper.hpp>

namespace threedimutil
{
    inline void draw_points(const Eigen::MatrixXd& P)
    {
        assert(P.cols() == 3);
        glBegin(GL_POINTS);
        for (int i = 0; i < P.rows(); ++ i)
        {
            threedimutil::vertex_3d(P.row(i));
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
            threedimutil::color_3d(C.row(i));
            threedimutil::vertex_3d(P.row(i));
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
            threedimutil::vertex_3d(P.row(E(i, 0)));
            threedimutil::vertex_3d(P.row(E(i, 1)));
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
            threedimutil::vertex_3d(V.row(F(i, 0)));
            threedimutil::vertex_3d(V.row(F(i, 1)));
            threedimutil::vertex_3d(V.row(F(i, 2)));
        }
        glEnd();
    }
}

#endif /* draw_utils_hpp */
