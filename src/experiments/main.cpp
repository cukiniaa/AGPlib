
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <agp/algo/baumgartner_algorithm.h>
#include <agp/algo/couto_algorithm.h>
#include <agp/algo/fisk_algorithm.h>
#include <agp/algo/ghosh_algorithm.h>
#include <agp/discretization_methods.h>
#include <agp/initial_placements.h>

#include <dirent.h>

void instruction(const std::string &message = "") {
    if (!message.empty()) {
        std::cerr << "ERROR: " << message << "\n";
    }
    std::vector<std::string> alg_flags = {"-fisk", "-ghosh", "-couto",
                                          "-baumgartner"};
    std::cerr << "./main [instances_dir] [algorithm_flag] "
                 "[output_file]\nAvailable algorithm flags ";
    for (const auto &flag : alg_flags)
        std::cerr << flag << " ";
    std::cerr << "\n";
}

std::vector<std::string> read_directory(const std::string &name) {
    std::vector<std::string> files;
    DIR *dirp = opendir(name.c_str());
    if (!dirp) {
        instruction("instances_dir \"" + name + "\" doesn't exist");
    }
    struct dirent *dp;
    while ((dp = readdir(dirp)) != nullptr) {
        if (dp->d_name == "" || dp->d_name[0] == '.')
            continue;
        files.push_back(name + "/" + dp->d_name);
    }
    closedir(dirp);
    return files;
}

void test_case(const std::string &filename,
               std::vector<Kernel::Point_2> &points) {
    std::ifstream in(filename);
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        Kernel::Point_2 p;
        in >> p;
        points.push_back(p);
    }
}

void output_results(const std::map<unsigned, double> &times,
                    std::ostream &out) {
    for (const auto &t : times) {
        out << t.first << " " << t.second << "\n";
    }
}

std::vector<Kernel::Point_2> find_guards(std::vector<Kernel::Point_2> &points,
                                         const std::string &alg_name) {
    if (alg_name == "-ghosh")
        return ghosh_algorithm(points);
    else if (alg_name == "-couto")
        return couto_algorithm(points, all_vertices);
    else if (alg_name == "-baumgartner")
        return baumgartner_algorithm(points, every_second, 300);
    else if (alg_name == "-fisk")
        return fisk_algorithm(points);
    else {
        instruction();
        exit(-1);
    }
}

void run_experiment(const std::string &dir_path, const std::string &algorithm,
                    const std::string &output_file) {

    std::ofstream out(output_file), ef("exceptions.out");
    std::map<unsigned, std::vector<double>> results;
    auto files = read_directory(dir_path);
    std::cout << "Instances from directory " << dir_path << ": " << files.size()
              << "\n";

    int i = 0;
    for (const auto &f : files) {
        try {
            std::vector<Kernel::Point_2> points;
            test_case(f, points);

            std::cout << ++i << "/" << files.size() << " " << f << ": "
                      << points.size() << " vertices.\n";

            auto t1 = std::chrono::high_resolution_clock::now();
            auto guards = find_guards(points, algorithm);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto time_span =
                std::chrono::duration_cast<std::chrono::duration<double>>(t2 -
                                                                          t1);

            out << f << " " << points.size() << " " << guards.size() << " "
                << time_span.count() << "\n";

            auto n = points.size();
            if (results.find(n) == results.end()) {
                results[n] = std::vector<double>();
            }
            results[n].push_back(time_span.count());
        } catch (const std::exception &e) {
            std::cerr << "Exception e caught!\n";
            std::cerr << e.what() << "\n";
            ef << f << " " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Exception caught \n";
            ef << f << "\n";
        }
    }

    out.close();
    ef.close();

    /// Compute mean time
    std::map<unsigned, double> times;
    for (auto &result : results) {
        double sum = 0;
        for (const auto &r : result.second) {
            sum += r;
        }
        times[result.first] = sum / (double)result.second.size();
        std::cout << result.first << " vertices, " << result.second.size()
                  << " instances, avg time: " << times[result.first] << "\n";
    }

    std::cout << "---------------------\n";

    // std::ofstream out_avg(avg_output_file);
    // output_results(times, out_avg);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        instruction();
        exit(-1);
    }

    std::string dir_name = argv[1], algorithm = argv[2], output_file = argv[3];

    run_experiment(dir_name, algorithm, output_file);

    return 0;
}
