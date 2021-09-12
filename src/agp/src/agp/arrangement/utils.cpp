#include <agp/arrangement/utils.h>

Polygon_2 create_polygon(const std::vector<Kernel::Point_2> &points) {
    return Polygon_2(points.begin(), points.end());
}

Polygon_2 arrangement_to_polygon(const Arrangement_2 &arr) {
    std::list<Kernel::Point_2> region_points;
    for (auto it = arr.vertices_begin(); it != arr.vertices_end(); ++it) {
        Kernel::Point_2 new_point(it->point().x(), it->point().y());
        region_points.emplace_back(new_point);
    }
    return Polygon_2(region_points.begin(), region_points.end());
}

std::vector<Polygon_2>
arrangements_to_polygons(const std::vector<Arrangement_2> &arrangements) {
    std::vector<Polygon_2> polygons;
    for (const auto &arr : arrangements) {
        polygons.emplace_back(arrangement_to_polygon(arr));
    }
    return polygons;
}

std::vector<Kernel::Point_2>
face_vertices(const Arrangement_2::Face_const_handle &face) {
    std::vector<Kernel::Point_2> vertices;
    Arrangement_2::Ccb_halfedge_const_circulator circ = face->outer_ccb(),
                                                 curr = circ;
    auto v = curr->source()->point();
    do {
        vertices.push_back(v);
        v = curr->target()->point();
    } while (++curr != circ);
    return vertices;
}
