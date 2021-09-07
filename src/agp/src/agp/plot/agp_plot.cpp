#include <agp/plot/agp_plot.h>

void print_points (const std::vector<Kernel::Point_2> &points, std::ostream &out) {
    for(const auto &p: points) {
        out << p << std::endl;
    }
}

void plot_polygon (const std::vector<Kernel::Point_2> &polygon, std::ostream &out) {
    out << "pol " << polygon.size() << "\n";
    print_points(polygon, out);
}

void plot_guard (Kernel::Point_2 g, std::ostream &out) {
    out << "grd " << g <<"\n";
}

void plot_guards (const std::vector<Kernel::Point_2> &guards, std::ostream &out) {
    for (const auto &g: guards) {
        plot_guard(g, out);
    }
}

void plot_region (const std::vector<Kernel::Point_2> &polygon, std::ostream &out) {
    out << "rgn " << polygon.size() << "\n";
    print_points(polygon, out);

}

void plot_region (const Arrangement_2 &region, std::ostream &out) {
    out << "rgn " << region.number_of_vertices() << "\n";
    for(auto v = region.vertices_begin(); v != region.vertices_end(); ++v) {
        out << v->point() << "\n";
    }
}
