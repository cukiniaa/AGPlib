
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
// #include <filesystem>
#include <chrono>
#include <map>
#include <numeric>

#include <agp/algo/ghosh_algorithm.h>
#include <agp/algo/fisk_algorithm.h>
#include <agp/algo/couto_algorithm.h>
#include <agp/algo/baumgartner_algorithm.h>
#include <agp/discretization_methods.h>
#include <agp/initial_placements.h>

#include <dirent.h>

void instruction(const std::string &message = "") {
    if (message != "") std::cerr << "ERROR: " << message << "\n";
    std::vector<std::string> alg_flags = { "-fisk", "-ghosh", "-couto", "-baumgartner" };
    std::cerr << "./main.cpp [instances_dir] [algorithm_flag] [output_file]\nAvailable algorithm flags ";
    for (const auto &flag: alg_flags)
        std::cerr << flag << " ";
    std::cerr << "\n";
}

std::vector<std::string> read_directory(const std::string& name) {
    std::vector<std::string> files;
    DIR* dirp = opendir(name.c_str());
    if (!dirp) instruction("instances_dir \"" + name + "\" doesn't exist");
    struct dirent * dp;
    while ((dp = readdir(dirp)) != nullptr) {
        if (dp->d_name == "" || dp->d_name[0] == '.') continue;
        files.push_back(name  + "/" + dp->d_name);
    }
    closedir(dirp);
    return files;
}

void test_case (const std::string &filename, std::vector<Kernel::Point_2> &points) {
    std::ifstream in(filename);
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        Kernel::Point_2 p;
        in >> p;
        points.push_back(p);
    }
}

void output_results (const std::map<unsigned, double> &times, std::ostream &out) {
    for(const auto &t: times) {
        out << t.first << " " << t.second << "\n";
    }
}


std::vector<Kernel::Point_2> find_guards (std::vector<Kernel::Point_2> &points, const std::string &alg_name) {
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


void time_experiment (const std::string &dir_path, const std::string &algorithm, const std::string &output_file) {
    // for(auto& p: std::filesystem::directory_iterator(dir_path))
    //    std::cout << p << '\n';


    std::ofstream ef;
    ef.open ("exceptions.out");
    std::map<unsigned, std::vector<double>> results;
    auto files = read_directory(dir_path);
    std::cerr << "Instances from directory " << dir_path << ": " << files.size() << "\n";
    int i = 0;
    for(const auto &f: files) {
        try {
            i++;
            std::vector<Kernel::Point_2> points;
            test_case(f, points);
            std::cerr << i << "/" << files.size() << " " << f << ": " << points.size() << " vertices.\n";
            std::cout << f << " " << points.size() << " ";
            auto t1 = std::chrono::high_resolution_clock::now();
            find_guards(points, algorithm);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

            std::cout << time_span.count() << "\n";

            auto n = points.size();
            if (results.find(n) == results.end()) results[n] = std::vector<double>();
            results[n].push_back(time_span.count());
        } catch (const std::exception& e) {
            std::cerr << "Exception e caught!\n";
            std::cerr << e.what() << "\n";
            ef << f << " " << e.what() << "\n";
            std::cout << "\n";
        } catch (...) {
            std::cerr << "Exception caught \n";
            ef << f << "\n";
            std::cout << "\n";
        }
    }

    ef.close();

    std::map<unsigned, double> times;
    for (auto &result : results) {
        std::cerr << result.first << " vertices, " << result.second.size() << " instances\n";
        double sum = 0;
        for(const auto &r: result.second)
            sum += r;
        times[result.first] = sum / (double) result.second.size();
    }

    std::cerr << "---------------------\n";

    output_results(times, std::cerr);
}

void iterations_and_solutions (const std::string &dir_path, const std::string &algorithm) {
    std::map<unsigned, std::vector<double>> results;
    auto files = read_directory(dir_path);
    std::cout << "Instances from directory " << dir_path << ": " << files.size() << "\n";
    int i = 1;
    for(const auto &f: files) {
        std::vector<Kernel::Point_2> points;
        test_case(f, points);
        std::cout << i++ << "/" << files.size()  << " " << f << ": " << points.size() << " vertices.\n";
        std::cerr << f << " ";
        find_guards(points, algorithm);
    }
}

int main (int argc, char *argv[]) {
    if (argc < 4) {
        instruction();
        exit(-1);
    }

    std::string dir_name = argv[1], algorithm = argv[2], output_file = argv[3];

    time_experiment(dir_name, algorithm, output_file);
    // iterations_and_solutions(dir_name, algorithm);

    // std::ifstream in(filename);
    // std::vector<Kernel::Point_2> kp;
    /*
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        Kernel::Point_2 p;
        in >> p;
        kp.push_back(p);
    }
*/
    /*
    if (!in.eof()) {
        int m;
        in >> m;
        for (int i = 0; i < m; ++i) {}
    }
    */
/*
    if (algorithm == "-fisk") {
        auto output = fisk(kp);
        plot_fisk(output, std::cerr);
    } else {
        auto guards = find_guards(kp, algorithm);
        plot_polygon(kp, std::cerr);
        plot_guards(guards, std::cerr);
    }
*/
    return 0;
}
