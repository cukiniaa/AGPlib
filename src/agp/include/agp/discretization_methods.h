/*! \file discretization_methods.h
 * \brief Provides initial discretization methods described in Couto et al.
 */
#ifndef AGP_DISCRETIZATION_METHODS_H
#define AGP_DISCRETIZATION_METHODS_H

#include <agp/arrangement/utils.h>

/*! \fn std::vector<Kernel::Point_2> single_vertex(const
 * std::vector<Kernel::Point_2> &vertices) Discretizes a polygon to one vertex
 * @param vertices of a given polygon
 * @return vector consisting of one of the vertices
 */
std::vector<Kernel::Point_2>
single_vertex(const std::vector<Kernel::Point_2> &vertices);

/*! \fn std::vector<Kernel::Point_2> all_vertices(const
 * std::vector<Kernel::Point_2> &vertices) Discretizes a polygon to all vertices
 * @param vertices of a given polygon
 * @return vector consisting of all vertices
 */
std::vector<Kernel::Point_2>
all_vertices(const std::vector<Kernel::Point_2> &vertices);

/*! \fn std::vector<Kernel::Point_2> discretization_min_dist(const
 * std::vector<Kernel::Point_2> &) Discretization as described in "An exact and
 * efficient algorithm for the orthogonal art gallery problem" by Couto et al.
 * Requirements: a given polygon is orthogonal. Other methods are preferred over
 * this one, as it greatly depends on the area of the polygon.
 * @param vertices of a given polygon
 * @return vector of points that form a grid inside the polygon
 */
std::vector<Kernel::Point_2>
discretization_min_dist(const std::vector<Kernel::Point_2> &vertices);

#endif // AGP_DISCRETIZATION_METHODS_H
