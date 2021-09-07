#include "agp/arrangement/partition.h"
#include <agp/plot/agp_plot.h>

void kernel_points_to_trait_points (const std::vector<Kernel::Point_2> &points, std::vector<Partition_Traits::Point_2> &ret) {
    for(const auto &p: points) {
        ret.push_back(Partition_Traits::Point_2(p.x(), p.y()));
    }
}

void to_convex_components (const std::vector<Kernel::Point_2> &k_points, std::list<Partition_Polygon_2> &partition_polys) {
    Partition_Traits partition_traits;
    std::vector<Partition_Traits::Point_2> points;
    kernel_points_to_trait_points(k_points, points);
    auto v_begin = points.begin();
    auto v_end = points.end();
    CGAL::approx_convex_partition_2(v_begin, v_end, std::back_inserter(partition_polys), partition_traits);
}

std::list<Kernel::Point_2> discretize_reflex (const std::vector<Kernel::Point_2> &v) {
    std::list<Kernel::Point_2> points;
    std::list<Partition_Polygon_2> partition_polys;
    to_convex_components(v, partition_polys);
    for (const auto &pol: partition_polys) {
        auto c = CGAL::centroid(pol.vertices_begin(), pol.vertices_end());
        points.push_back(Kernel::Point_2(c.x(), c.y()));
    }
    return points;
}

std::list<Kernel::Point_2> arr_discretization (const Arrangement_2 &arr) {
    std::list<Kernel::Point_2> points;
    for (auto f = arr.faces_begin(); f != arr.faces_end(); ++f) {
        if (f->is_unbounded()) continue;
        auto to_add = discretize_reflex(face_vertices(f));
        points.insert(points.end(), to_add.begin(), to_add.end());
    }
    return points;
}
