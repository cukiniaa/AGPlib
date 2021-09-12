/*!
 *  @defgroup baumgartner
 *  @{
 *  \file baumgartner_algorithm.h
 *  \brief An iterative method described in "Exact solutions and bounds for
 * general art gallery problems" by Baumgartner et al. \page baumgartner_algo
 * Baumgartner \brief An iterative method described in "Exact solutions and
 * bounds for general art gallery problems" by Baumgartner et al.
 *
 *  The algorithm tackles the point art gallery problem. It tries to find upper
 * and lower bounds of the number of guards until their convergence using linear
 * programming.
 */

#ifndef AGP_BAUMGARTNER_ALGORITHM_H
#define AGP_BAUMGARTNER_ALGORITHM_H

#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/centroid.h>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>
#include <agp/cplex/cplex.h>
#include <agp/initial_placements.h>

#include <ctime>
#include <functional>

using Traits_2 = CGAL::Arr_segment_traits_2<Kernel>;
using vec_t = std::vector<Kernel::Point_2>;

/*! \fn vec_t baumgartner_algorithm(const vec_t &vertices,
 * std::function<wg_placement(const vec_t &)> initial_placement_method) \brief
 * Potential guards set G and witnesses set W are chosen. Problem relaxation
 * AGR(G, W) is solved using linear programing to obtain an optimal solution
 * from the set G, such that all points from W are guarded. The approach is
 * repeated until new witnesses or new potential guards are found. Such solution
 * solves general AGP, where guards can be placed inside the polygon.
 * @param vertices - vector of vertices representing a gallery
 * @param initial_placement_method - method that chooses potential guards and
 * witnesses
 * @param sec - number of seconds after which the algorithm should be halted
 * @return vector of guards or an empty vector if no guards were found within
 * given time
 */

vec_t baumgartner_algorithm(
    const vec_t &vertices,
    std::function<wg_placement(const vec_t &)> initial_placement_method,
    int sec);

#endif // AGP_BAUMGARTNER_ALGORITHM_H
/** @} */
