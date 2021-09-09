/*!
 *  @defgroup couto
 *  @{
 * \file couto_algorithm.h
 *  \brief Couto's et al. approach to the AGP described in "An exact algorithm for minimizing vertex guards on art galleries".
 *  \page couto_algo Couto
 *  \brief Couto's et al. approach to the AGP described in "An exact algorithm for minimizing vertex guards on art galleries".
 */

#ifndef AGP_EXACT_ALGORITHM_H
#define AGP_EXACT_ALGORITHM_H

#include <list>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/centroid.h>
#include <CGAL/Kernel/global_functions.h>
#include <algorithm>
#include <ctime>
#include <functional>

#include <agp/cplex/cplex.h>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>
#include <agp/arrangement/partition.h>

using Polygon_2 = CGAL::Polygon_2<Kernel>;
using Polygon_with_holes_2 = CGAL::Polygon_with_holes_2<Kernel>;
using vec_t = std::vector<Kernel::Point_2>;

/*! \fn vec_t couto_algorithm(const vec_t &vertices, std::function<vec_t(const vec_t &)> discretization_method)
 *  The method discretizes the polygon and solves the vertex AGP using integer programming for a current discretization.
 *  Each time an uncovered region is found, a point representing that region is added to the discretization and the IP model is solved again.
 *  The method uses Triangular_expansion_visibility_2 class to compute visibility regions for all vertices. The IP model is solved by the CPLEX package.
 *  The method provides an optimal solution for the vertex AGP.
 * @param vertices - vector of vertices representing a gallery
 * @param discretization_method - method used to get the initial discretization of the polygon
 * @return vector of guards
 */
vec_t couto_algorithm(const vec_t &vertices, const std::function<vec_t(const vec_t &)>& discretization_method);

#endif //AGP_EXACT_ALGORITHM_H
/** @} */
