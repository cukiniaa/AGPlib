#include <agp/discretization_methods.h>

std::vector<Kernel::Point_2>
single_vertex(const std::vector<Kernel::Point_2> &points) {
    return {points[0]};
}

std::vector<Kernel::Point_2>
all_vertices(const std::vector<Kernel::Point_2> &points) {
    return points;
}

std::vector<Kernel::Point_2>
discretization_min_dist(const std::vector<Kernel::Point_2> &points) {
    std::vector<Kernel::FT> x;
    std::vector<Kernel::FT> y;
    for (const auto &p : points) {
        x.push_back(p.x());
        y.push_back(p.y());
    }
    std::sort(x.begin(), x.end());
    std::sort(y.begin(), y.end());

    Kernel::FT delta_x = x[x.size() - 1] - x[0];
    Kernel::FT delta_y = y[y.size() - 1] - y[0];
    Kernel::FT min_dist_x = delta_x;
    Kernel::FT min_dist_y = delta_y;

    for (unsigned i = 1; i < points.size() - 2; ++i) {
        Kernel::FT dist = points[i + 1].x() - points[i].x();
        if (dist > 0 && dist < min_dist_x) {
            min_dist_x = dist;
        }
        dist = points[i + 1].y() - points[i].y();
        if (dist > 0 && dist < min_dist_y) {
            min_dist_y = dist;
        }
    }

    Kernel::Point_2 last = Kernel::Point_2(x[0], y[0]);
    std::set<Kernel::Point_2> discretization_set;

    while (last.y() <= y.back()) {
        while (last.x() <= x.back()) {
            discretization_set.insert(Kernel::Point_2(last));
            last = Kernel::Point_2(last.x() + min_dist_x, last.y());
        }
        last = Kernel::Point_2(x[0], last.y() + min_dist_y);
    }

    discretization_set.insert(points.begin(), points.end());
    std::vector<Kernel::Point_2> discretization(discretization_set.begin(),
                                                discretization_set.end());

    return discretization;
}
