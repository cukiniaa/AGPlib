#include "agp/arrangement/visibility.h"
// #define NDEBUG
#include <cassert>


bool point_visible (const Arrangement_2 &arr, const Kernel::Point_2 &point) {
    CGAL::Arr_walk_along_line_point_location<Arrangement_2> pl(arr);
    auto result = pl.locate(point);
    const Arrangement_2::Face_const_handle* f;
    if ((f = boost::get<Arrangement_2::Face_const_handle>(&result))) {
        return !(*f)->is_unbounded();
    }
    return true;
}


Arrangement_2 vertex_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &point) {
    auto he = polygon.halfedges_begin();
    while (he->target()->point() != point || he->face()->is_unbounded())
        he++;
    Arrangement_2 output;
    typedef CGAL::Triangular_expansion_visibility_2<Arrangement_2>  TEV;
    TEV tev(polygon);
    tev.compute_visibility(point, he, output);
    return output;
}


Arrangement_2 edge_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &point, const Arrangement_2::Halfedge_const_handle &he) {
    Arrangement_2 output;
    typedef CGAL::Triangular_expansion_visibility_2<Arrangement_2>  TEV;
    TEV tev(polygon);
    tev.compute_visibility(point, he, output);
    return output;
}


Arrangement_2 general_point_visibility_region (const Arrangement_2 &polygon, const Kernel::Point_2 &point) {
    CGAL::Arr_walk_along_line_point_location<Arrangement_2> pl(polygon);
    auto result = pl.locate(point);
    if (boost::get<Arrangement_2::Vertex_const_handle>(&result))
        return vertex_visibility_region(polygon, point);
    else if (auto e = boost::get<Arrangement_2::Halfedge_const_handle>(&result)) {
        if ((*e)->face()->is_unbounded()) return edge_point_visibility_region(polygon, point, (*e)->twin());
        else return edge_point_visibility_region(polygon, point, *e);
    }
    return inner_point_visibility_region(polygon, point);
}


Arrangement_2 inner_point_visibility_region(const Arrangement_2 &polygon, const Kernel::Point_2 &point) {
    assert(polygon.number_of_faces() == 2);

    Arrangement_2::Face_const_handle f = polygon.faces_begin();
    for(; f != polygon.faces_end(); ++f) {
        if(!f->is_unbounded()) break;
    }

    Arrangement_2 output;
    typedef CGAL::Triangular_expansion_visibility_2<Arrangement_2>  TEV;
    TEV tev(polygon);
    tev.compute_visibility(point, f, output);
    return output;
}

std::vector<Arrangement_2> all_visibility_regions (const Arrangement_2 &polygon, const std::vector<Kernel::Point_2> &points) {
    std::vector<Arrangement_2> visibility_regions(points.size());
    Arrangement_2::Halfedge_const_handle he = polygon.halfedges_begin();
    while (he->target()->point() != points[0] || he->source()->point() != points[points.size()-1])
        he++;

    typedef CGAL::Triangular_expansion_visibility_2<Arrangement_2>  TEV;
    TEV tev(polygon);
    for (unsigned i = 0; i < points.size(); ++i) {
        Arrangement_2 output;
        tev.compute_visibility(points[i], he, output);
        visibility_regions[i] = output;
        he = he->next();
    }
    return visibility_regions;
}
