#include <agp/algo/ghosh_algorithm.h>

struct fan {
    unsigned long point_index;
    std::set<Kernel::Point_2> components;
    bool operator<(const fan& rhs) const {
        return components.size() < rhs.components.size();
    }
};

void create_fans(const std::set<Kernel::Point_2> &convex_components, const std::vector<Arrangement_2> &visibility_regions, std::vector<fan> &fans);

std::vector<Kernel::Point_2> get_guards(const std::vector<unsigned long> &indices,  const std::vector<Kernel::Point_2> &points);


std::vector<Kernel::Point_2> ghosh_algorithm (const std::vector<Kernel::Point_2> &points) {
    Arrangement_2 polygon = create_arrangement<Arrangement_2>(points);
    std::vector<Arrangement_2> visibility_regions = all_visibility_regions(polygon, points);
    Arrangement_2 convex_components_arrangement = overlay_arrangements<Arrangement_2>(polygon, visibility_regions);
    std::set<Kernel::Point_2> convex_components;
    std::vector<fan> fans;
    std::vector<unsigned long> approx_solution;

    for (auto f = convex_components_arrangement.faces_begin(); f != convex_components_arrangement.faces_end(); ++f) {
        if (f->is_unbounded()) continue;
        auto vertices = face_vertices(f);
        auto c = CGAL::centroid(vertices.begin(), vertices.end());
        convex_components.insert(c);
    }

//    unsigned long n = points.size();

    create_fans(convex_components, visibility_regions, fans);

//    for (unsigned long i = 0; i < n; ++i) {
//        std::set<Kernel::Point_2> visible_components;
//        for (const auto &convex_component : convex_components) {
//            if (point_visible(visibility_regions[i], convex_component))
//                visible_components.insert(convex_component);
//        }
//        fans.push_back({ i, visible_components });
//    }

    while (!convex_components.empty()) {
        auto it = std::max_element(fans.begin(), fans.end());
        fan current_biggest = *it;
        fans.erase(it);

        approx_solution.push_back(current_biggest.point_index);
        for (auto &p : current_biggest.components) {
            convex_components.erase(p);
            for (auto &other: fans) {
                 other.components.erase(p);
            }
        }
    }

//    std::vector<Kernel::Point_2> solution;
//    for (const auto &g: approx_solution) {
//        solution.push_back(points[g]);
//    }

//     std::cout << solution.size() << " ";
    return get_guards(approx_solution, points); // solution;
}


void create_fans(const std::set<Kernel::Point_2> &convex_components, const std::vector<Arrangement_2> &visibility_regions, std::vector<fan> &fans) {
    unsigned long n = visibility_regions.size();
    for (unsigned long i = 0; i < n; ++i) {
        std::set<Kernel::Point_2> visible_components;
        for (const auto &convex_component : convex_components) {
            if (point_visible(visibility_regions[i], convex_component))
                visible_components.insert(convex_component);
        }
        fans.push_back({ i, visible_components });
    }
}


std::vector<Kernel::Point_2> get_guards(const std::vector<unsigned long> &indices,  const std::vector<Kernel::Point_2> &points) {
    std::vector<Kernel::Point_2> solution;
    for (const auto &i: indices) {
        solution.push_back(points[i]);
    }
    return solution;
}
