/*! \file visibility.h
 * \brief A module for computing visibility regions and querying visibility.
 */
#ifndef AGP_ARRANGEMENT_VISIBILITY_H
#define AGP_ARRANGEMENT_VISIBILITY_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Triangular_expansion_visibility_2.h>
#include <CGAL/Arr_walk_along_line_point_location.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel   Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                  Traits;
typedef CGAL::Arrangement_2<Traits>                         Arrangement_2;

/*! \fn bool point_visible (const Arrangement_2 &arr, const Kernel::Point_2 &point)
 * Given two points \f$ p, g \f$, answers a question if \f$ q \in \mathcal{V}(p) \f$.
 * @param arr - visibility region of p
 * @param point - point q
 * @return true if point is visible, false otherwise
 */
bool point_visible (const Arrangement_2 &arr, const Kernel::Point_2 &point);

/*! \fn Arrangement_2 general_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p)
 * Computes visibility region of a point \f$ p \f$ from a given polygon. No a priori knowledge is needed apart from the fact that the point must lie inside the polygon.
 * @param polygon - given polygon
 * @param p - point which visibility region is computed
 * @return visibility region of \f$ p \f$
 */
Arrangement_2 general_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p);

/*! \fn Arrangement_2 vertex_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p)
 * Computes visibility region of a point \f$ p \f$ from a given polygon. Point \f$ p \f$ must be a vertex of the given polygon.
 * This method if preferred over general_point_visibility_region if it's known in advance that \f$ p \f$ is one of the vertices.
 * @param polygon - given polygon
 * @param p - point which visibility region is computed
 * @return visibility region of \f$ p \f$
 */
Arrangement_2 vertex_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p);

/*! \fn Arrangement_2 inner_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p)
 * Computes visibility region of a point \f$ p \f$ from a given polygon. Point \f$ p \f$ must lie in the interior of the given polygon.
 * This method if preferred over general_point_visibility_region if it's known in advance that \f$ p \in Int(P) \f$.
 * @param polygon - given polygon
 * @param p - point which visibility region is computed
 * @return visibility region of \f$ p \f$
 */
Arrangement_2 inner_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &p);

/*! \fn std::vector<Arrangement_2> all_visibility_regions (const Arrangement_2 &polygon, const std::vector<Kernel::Point_2> &points)
 * Computes visibility regions of a vector of points using method general_point_visibility_region.
 * It is required that all the given points lie inside the polygon, can be either vertices or lie on the edges or in polygon's interior.
 * @param polygon - polygon which contains all the points.
 * @param points - vector of points which visibility regions are computed
 * @return vector of visibility regions computed for all the points
 */

std::vector<Arrangement_2> all_visibility_regions (const Arrangement_2 &polygon, const std::vector<Kernel::Point_2> &points);

#endif //AGP_ARRANGEMENT_VISIBILITY_H
