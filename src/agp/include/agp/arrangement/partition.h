/*! \file partition.h
 * \brief Provides functions for partitioning a polygon - triangulation and
 * fracture to convex components.
 *
 * Uses Triangulation_2 and Partition_Polygon_2 clases from CGAL.
 */
#ifndef AGP_ARRANGEMENT_PARTITION_H
#define AGP_ARRANGEMENT_PARTITION_H

#include <CGAL/Partition_is_valid_traits_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/centroid.h>
#include <CGAL/partition_2.h>
#include <CGAL/polygon_function_objects.h>
#include <agp/arrangement/utils.h>

#include <list>

using Partition_Traits = CGAL::Partition_traits_2<Kernel>;
using Partition_Polygon_2 = Partition_Traits::Polygon_2;
using Triangulation = CGAL::Triangulation_2<Kernel>;

/*! \fn void to_convex_components (const std::vector<Kernel::Point_2> &vertices,
 * std::list<Partition_Polygon_2> &output) \brief Divides a polygon into convex
 * components.
 * @param vertices - vector of vertices representing a polygon
 * @param output - list of convex components of the given polygon
 */
void to_convex_components(const std::vector<Kernel::Point_2> &vertices,
                          std::list<Partition_Polygon_2> &output);

/*! \fn std::list<Kernel::Point_2> arr_discretization (const Arrangement_2 &arr)
 *  \brief Discretizes a simple polygon given as an instance of the
 * Arrangement_2 class from CGAL. It divides the polygon into convex components
 * and returns their centroids.
 *  @param arr - simple polygon represented as an Arrangement_2
 *  @return list of vertices discretizing the given polygon
 */
std::list<Kernel::Point_2> arr_discretization(const Arrangement_2 &arr);

/*! \fn Arrangement triangulate (const std::vector<Kernel::Point_2> &vertices)
 * \brief Triangulates a given polygon using a build-in method from CGAL.
 * Vertices are expected to be in counterclockwise orientation.
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param vertices - vertices of a simple polygon
 * @return triangulated arrangement
 */
template <typename Arrangement>
Arrangement triangulate(const std::vector<Kernel::Point_2> &vertices) {
    std::list<Partition_Polygon_2> partition_polys;
    to_convex_components(vertices, partition_polys);

    std::vector<Arrangement> triangles;
    Triangulation t;
    t.insert(vertices.begin(), vertices.end());

    for (const auto &part : partition_polys) {
        Triangulation t;
        t.insert(part.vertices_begin(), part.vertices_end());
        for (auto face = t.finite_faces_begin(); face != t.finite_faces_end();
             ++face) {
            std::vector<Kernel::Point_2> vertices = {face->vertex(0)->point(),
                                                     face->vertex(1)->point(),
                                                     face->vertex(2)->point()};
            triangles.push_back(create_arrangement<Arrangement>(vertices));
        }
    }

    return overlay_arrangements<Arrangement>(Arrangement(), triangles);
}

#endif // AGP_ARRANGEMENT_PARTITION_H
