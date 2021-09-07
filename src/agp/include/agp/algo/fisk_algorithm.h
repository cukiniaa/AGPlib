/*!
 *  @defgroup fisk
 *  @{
 *  \file fisk_algorithm.h
 *  \brief An algorithm based on Fisk's approach to the AGP described in "A short proof of Chvátal’s watchman theorem" S.Fisk.
 *  \page fisk_algo Fisk
 *  \brief An algorithm based on Fisk's approach to the AGP described in "A short proof of Chvátal’s watchman theorem" S.Fisk.
 *
 *  The approach makes use of the triangulation method from CGAL library (<a href="https://doc.cgal.org/latest/Triangulation_2/index.html">details here</a>).
 */

#ifndef AGP_FISK_H
#define AGP_FISK_H

#include <queue>
#include <CGAL/Arr_extended_dcel.h>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/partition.h>

using Traits_2 = CGAL::Arr_segment_traits_2<Kernel>;
using Dcel = CGAL::Arr_extended_dcel<Traits_2, CGAL::Color, bool, int>;
using Extended_Arrangement_2 = CGAL::Arrangement_2<Traits_2, Dcel>;

/*! \fn Extended_Arrangement_2 fisk_arrangement (const std::vector<Kernel::Point_2> &vertices)
 *  \brief The method provides the triangulated arrangement with coloured vertices.
 *  The algorithm uses triangulation from the CGAL package Triangulation_2 and breadth-first search method to colour triangles.
 * @param vertices - vector of vertices representing a gallery
 * @return triangulated and coloured arrangement
 */
Extended_Arrangement_2 fisk_arrangement (const std::vector<Kernel::Point_2> &vertices);

/*! \fn std::vector<Kernel::Point_2> fisk_algorithm (const std::vector<Kernel::Point_2> &vertices)
 *  \brief The method is based on Fisk's proof that \f$ \lfloor \frac{n}{3} \rfloor \f$ of guards always suffice and are sometimes necessary to guard a gallery.
 *  The algorithm uses triangulation and breadth-first search method to colour triangles and find guards.
 *  \f$ \lfloor \frac{n}{3} \rfloor \f$ guards are returned, all placed in polygon's vertices.
 * @param vertices - vector of vertices representing a gallery
 * @return vector of guards
 */
std::vector<Kernel::Point_2> fisk_algorithm (const std::vector<Kernel::Point_2> &vertices);

#endif //AGP_FISK_H

/** @} */
