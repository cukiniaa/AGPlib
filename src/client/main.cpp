
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include <agp/algo/ghosh_algorithm.h>
#include <agp/algo/fisk_algorithm.h>
#include <agp/algo/couto_algorithm.h>
#include <agp/algo/baumgartner_algorithm.h>
#include <agp/discretization_methods.h>
#include <agp/initial_placements.h>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>

std::vector<std::string> alg_flags = { "-fisk", "-ghosh", "-couto", "-baumgartner" };

void instruction() {
    std::cerr << "./main.cpp filename algorithm\nAvailable algorithm flags ";
    for (const auto &flag: alg_flags)
        std::cerr << flag << " ";
    std::cerr << "\n";
}

std::vector<Kernel::Point_2> find_guards (const std::vector<Kernel::Point_2> &points, const std::string &alg_name) {
    if (alg_name == "-ghosh")
        return ghosh_algorithm(points);
    else if (alg_name == "-couto")
        return couto_algorithm(points, all_vertices);
    else if (alg_name == "-baumgartner")
        return baumgartner_algorithm(points, every_second, 120);
    else if (alg_name == "-fisk")
        return fisk_algorithm(points);
    else {
        instruction();
        exit(-1);
    }
}

void process(const std::string &algorithm, const std::vector<Kernel::Point_2> &points) {
    auto polygon = create_arrangement<Arrangement_2>(points);
    if (algorithm == "-fisk-arr") {
        auto output = fisk_arrangement(points);
        plot_fisk(output, std::cerr);
    } else {
        auto guards = find_guards(points, algorithm);
        plot_polygon(points, std::cerr);
        plot_guards(guards, std::cerr);
        for (const auto &g: guards) {
            plot_region(general_point_visibility_region(polygon, g), std::cerr);
        }
    }

}

int main (int argc, char *argv[]) {
    if (argc < 3) {
        instruction();
        exit(-1);
    }

    std::string filename = argv[1], algorithm = argv[2];
    std::ifstream in(filename);
    std::vector<Kernel::Point_2> points;

    if (!in) {
        std::cout << "No such file: " << filename << std::endl;
        exit(-1);
    }

    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        Kernel::Point_2 p;
        in >> p;
        points.push_back(p);
    }

    process(algorithm, points);

    return 0;
}
