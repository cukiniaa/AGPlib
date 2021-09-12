/*! \file initial_placements.h
 * \brief Provides functions for initial placements of potential guards and
 * witnesses for Baumgartner's algorithm.
 */

#ifndef AGP_INITIAL_PLACEMENTS_H
#define AGP_INITIAL_PLACEMENTS_H

#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>

/*! \struct wg_placement
 * Used to store potential guards and witnesses used in \ref
 * baumgartner_algorithm.
 */
struct wg_placement {
    std::vector<Kernel::Point_2> guards;    /*!< potential guards */
    std::vector<Kernel::Point_2> witnesses; /*!< witnesses of visibility */
    std::vector<Arrangement_2>
        guard_visibility; /*!< regions of visibility corresponding to guards */
    std::vector<Arrangement_2> witness_visibility; /*!< regions of visibility
                                                    * corresponding to witnesses
                                                    */
};

/*! \fn wg_placement every_second(const std::vector<Kernel::Point_2> &vertices)
 * Returns two vectors - G (wg.guards) and W (wg.vertices) and their
 * corresponding visibility regions. Guards and witnesses are chosen
 * alternately.
 * @param vertices of a given polygon
 * @return an object with vectors of G, W and their visibility regions
 */
wg_placement every_second(const std::vector<Kernel::Point_2> &vertices);

#endif // AGP_INITIAL_PLACEMENTS_H
