#ifndef THREEDIMUTIL_MESH_HPP
#define THREEDIMUTIL_MESH_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace threedimutil
{
    template <typename Scalar, typename UInt>
    Eigen::Matrix<Scalar, 3, Eigen::Dynamic> CalcVertexNormals(const Eigen::Matrix<Scalar, 3, Eigen::Dynamic>& vertex_data,
                                                               const Eigen::Matrix<UInt, 3, Eigen::Dynamic>&   index_data)
    {
        Eigen::Matrix<Scalar, 3, Eigen::Dynamic> normal_data = Eigen::Matrix<Scalar, 3, Eigen::Dynamic>::Zero(vertex_data.rows(), vertex_data.cols());

        for (unsigned face = 0; face < index_data.cols(); ++face)
        {
            const auto& x_0 = vertex_data.col(index_data(0, face));
            const auto& x_1 = vertex_data.col(index_data(1, face));
            const auto& x_2 = vertex_data.col(index_data(2, face));

            const auto area_scaled_face_normal = (x_1 - x_0).cross(x_2 - x_0);

            normal_data.col(index_data(0, face)) += area_scaled_face_normal;
            normal_data.col(index_data(1, face)) += area_scaled_face_normal;
            normal_data.col(index_data(2, face)) += area_scaled_face_normal;
        }

        for (unsigned vertex = 0; vertex < vertex_data.cols(); ++vertex)
        {
            normal_data.col(vertex).normalize();
        }

        return normal_data;
    }
}

#endif /* THREEDIMUTIL_MESH_HPP */
