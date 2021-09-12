/*!
 *  @defgroup ghosh
 *  @{
 *  \file ghosh_algorithm.h
 *  \brief Ghosh's approach to the AGP, described in Approximation algorithms
 * for art gallery problems in polygons", S.Ghosh ". \page ghosh_algo Ghosh
 *  \brief Ghosh's approach to the AGP, described in Approximation algorithms
 * for art gallery problems in polygons", S.Ghosh ".
 */
#ifndef AGP_GHOSH_ALGORITHM_H
#define AGP_GHOSH_ALGORITHM_H

#include <CGAL/centroid.h>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>
#include <agp/plot/agp_plot.h>

#include <algorithm>
#include <ostream>
#include <set>

/*! \fn std::vector<Kernel::Point_2> ghosh_algorithm (const
 * std::vector<Kernel::Point_2> &vertices) \brief An approximation algorithm
 * that reduces the AGP to the set cover problem.
 * @param vertices - vector of vertices representing a gallery
 * @return vector of guards
 */
std::vector<Kernel::Point_2>
ghosh_algorithm(const std::vector<Kernel::Point_2> &vertices);

#endif // AGP_GHOSH_ALGORITHM_H

/** @} */
