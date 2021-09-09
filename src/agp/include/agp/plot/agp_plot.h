/*! \file agp_plot.h
 * \brief A module for plotting results.
 */
#ifndef AGP_AGP_PLOT_INPUT_H
#define AGP_AGP_PLOT_INPUT_H

#include <agp/arrangement/utils.h>
#include <ostream>

void print_points (const std::vector<Kernel::Point_2> &points, std::ostream &out);

void print_polygon (const std::vector<Kernel::Point_2> &polygon, std::ostream &out);

void print_guard (Kernel::Point_2 g, std::ostream &out);

void print_guards (const std::vector<Kernel::Point_2> &guards, std::ostream &out);

void print_region (const std::vector<Kernel::Point_2> &polygon, std::ostream &out);

void print_region (const Arrangement_2 &region, std::ostream &out);

template <typename Point>
void print_point (const Point &p, const CGAL::Color &c, std::ostream &out) {
    out << "pnt " << p << " " << c << "\n";
}

template <typename Arrangement>
void plot_fisk (const Arrangement &arr, std::ostream &out) {
    for (auto fit = arr.faces_begin(); fit !=  arr.faces_end(); ++fit) {
        if (fit->is_unbounded()) continue;
        out << "pol 3\n";
        auto circ = fit->outer_ccb(), curr = circ;
        do { out << curr->target()->point() << "\n";
        } while (++curr != circ);

        do {
            print_point(curr->target()->point(), curr->target()->data(), out);
        } while (++curr != circ);
    }
}

#endif //AGP_AGP_PLOT_INPUT_H
