/*! \file utils.h
 */
#ifndef AGP_POLYGON_UTILS_H
#define AGP_POLYGON_UTILS_H

#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_2_algorithms.h>

#include <list>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Traits = CGAL::Arr_segment_traits_2<Kernel>;
using Arrangement_2 = CGAL::Arrangement_2<Traits>;
using Polygon_2 = CGAL::Polygon_2<Kernel>;
using v_it = std::vector<Kernel::Point_2>::const_iterator;

/*! \fn Arrangement create_arrangement (const v_it &begin, const v_it &end)
 * \brief Creates an arrangement from points. Points are expected to be vertices
 * of a simple polygon.
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param begin - vertices iterator
 * @param end - vertices iterator
 * @return arrangement created from points
 */
template <typename Arrangement>
Arrangement create_arrangement(const v_it &begin, const v_it &end) {
    std::vector<Kernel::Segment_2> segments;
    auto it = begin;
    for (; std::next(it, 1) != end; ++it) {
        segments.push_back(Kernel::Segment_2(*it, *(it + 1)));
    }
    segments.push_back(Kernel::Segment_2(*it, *begin));

    Arrangement arr;
    CGAL::insert_non_intersecting_curves(arr, segments.begin(), segments.end());

    return arr;
}

/*! \fn Arrangement create_arrangement (const std::vector<Kernel::Point_2>
 * &vertices) \brief Creates an arrangement from points. Points are expected to
 * be vertices of a simple polygon.
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param vertices - vector of vertices
 * @return arrangement created from vertices
 */
template <typename Arrangement>
Arrangement create_arrangement(const std::vector<Kernel::Point_2> &vertices) {
    return create_arrangement<Arrangement>(vertices.begin(), vertices.end());
}

/*! \fn Polygon_2 create_polygon (const std::vector<Kernel::Point_2> &vertices)
 * \brief Constructs a simple polygon from given vertices.
 * @param vertices
 * @return an object of class Polygon_2 constructed from given vertices
 */
Polygon_2 create_polygon(const std::vector<Kernel::Point_2> &vertices);

/*! \fn Polygon_2 arrangement_to_polygon (const Arrangement_2 &arr)
 * \brief Converts an arrangement to polygon.
 * @param arr - given arrangement
 * @return polygon created from the given arrangement
 */
Polygon_2 arrangement_to_polygon(const Arrangement_2 &arr);

/*! \fn std::vector<Polygon_2> arrangements_to_polygons (const
 * std::vector<Arrangement_2> &arrangements) \brief Converts a vector of
 * arrangements to a vector of polygons.
 * @param arrangements - vector of arrangements
 * @return vector of corresponding polygons
 */
std::vector<Polygon_2>
arrangements_to_polygons(const std::vector<Arrangement_2> &arrangements);

/*! \fn std::vector<Kernel::Point_2> face_vertices (const
 * Arrangement_2::Face_const_handle &face) \brief Returns a vector of face's
 * vertices.
 * @param face - const handle to a face
 * @return vertices of the given face
 */
std::vector<Kernel::Point_2>
face_vertices(const Arrangement_2::Face_const_handle &face);

/*! \fn void print_ccb (typename Arrangement::Ccb_halfedge_const_circulator
 * circ) \brief Prints halfedges of a given connected component boundary.
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param circ - circulator that allows traversing the halfedges of the
 * connected component boundary
 */

template <typename Arrangement>
void print_ccb(typename Arrangement::Ccb_halfedge_const_circulator circ) {
    typename Arrangement::Ccb_halfedge_const_circulator curr = circ;
    std::cout << "(" << curr->source()->point() << ")";
    do {
        std::cout << " [" << curr->curve() << "] "
                  << "(" << curr->target()->point() << ")";
    } while (++curr != circ);
    std::cout << std::endl;
}

/*! \fn void print_face (const typename Arrangement::Face_const_handle &f)
 * \brief Prints the following information about a given face:
 *  - is unbounded/bounded
 *  - connected component boundary if is bounded
 *  - holes in the face
 *  - isolated vertices in the face
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param f - const handle to the face
 */
template <typename Arrangement>
void print_face(const typename Arrangement::Face_const_handle &f) {
    if (f->is_unbounded())
        std::cout << "Unbounded face. " << std::endl;
    else {
        std::cout << "Outer boundary: ";
        print_ccb<Arrangement>(f->outer_ccb());
    }

    typename Arrangement::Hole_const_iterator hi;
    int index = 1;
    for (hi = f->holes_begin(); hi != f->holes_end(); ++hi, ++index) {
        std::cout << " Hole #" << index << ": ";
        print_ccb<Arrangement>(*hi);
    }

    typename Arrangement::Isolated_vertex_const_iterator iv;
    for (iv = f->isolated_vertices_begin(), index = 1;
         iv != f->isolated_vertices_end(); ++iv, ++index) {
        std::cout << " Isolated vertex #" << index << ": "
                  << "(" << iv->point() << ")" << std::endl;
    }
}

/*! \fn void print_arrangement (const Arrangement &arr)
 * \brief Prints the following information about a given arrangement:
 *  - number of vertices, edges and faces
 *  - vertices with their degrees
 *  - edges in order of appearance
 *  - faces with their holes and ccbs
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param arr - given arrangement
 */
template <typename Arrangement> void print_arrangement(const Arrangement &arr) {
    std::cout << "-------------------------------------------\n";
    std::cout << "Arrangement " << std::endl
              << "   V = " << arr.number_of_vertices()
              << ",  E = " << arr.number_of_edges()
              << ",  F = " << arr.number_of_faces() << std::endl;

    std::cout << arr.number_of_vertices() << " vertices:" << std::endl;
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
        std::cout << "(" << vit->point() << ")";
        if (vit->is_isolated())
            std::cout << " - Isolated." << std::endl;
        else
            std::cout << " - degree " << vit->degree() << std::endl;
    }
    std::cout << arr.number_of_edges() << " edges:" << std::endl;
    for (auto eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
        std::cout << "[" << eit->curve() << "]" << std::endl;
    std::cout << arr.number_of_faces() << " faces:" << std::endl;
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); ++fit)
        print_face<Arrangement>(fit);
    std::cout << "-------------------------------------------\n";
}

/*! \fn Arrangement overlay_arrangements (const Arrangement &primary, const
 * std::vector<Arrangement> &arrangements) \brief Constructs an overlay of the
 * given arrangement starting from the primary arrangement.
 * @tparam Arrangement - a class of type CGAL::Arrangement_2
 * @param primary - primary arrangement
 * @param arrangements - vector of other arrangements to overlay
 * @return result of overlaid arrangements
 */
template <typename Arrangement>
Arrangement overlay_arrangements(const Arrangement &primary,
                                 const std::vector<Arrangement> &arrangements) {
    Arrangement overlay_arrangement = primary;
    for (const auto &arr : arrangements) {
        Arrangement arr_output;
        overlay(overlay_arrangement, arr, arr_output);
        overlay_arrangement = arr_output;
    }
    return overlay_arrangement;
}

#endif // AGP_POLYGON_UTILS_H
