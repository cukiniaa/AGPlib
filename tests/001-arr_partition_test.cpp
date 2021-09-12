#define CATCH_CONFIG_MAIN
#include <agp/arrangement/partition.h>
#include <agp/arrangement/utils.h>

#include <catch2/catch.hpp>

std::vector<Kernel::Point_2> get_instance_from_file(std::ifstream &in) {
    std::vector<Kernel::Point_2> points;
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        Kernel::Point_2 p;
        in >> p;
        points.push_back(p);
    }
    return points;
}

TEST_CASE("Triangulate a polygon (20 vertices)") {
    std::string filename = "./instances/rand-20-1.pol";
    std::ifstream in(filename);
    REQUIRE(in);
    auto points = get_instance_from_file(in);
    auto t = triangulate<Arrangement_2>(points);
    REQUIRE(t.number_of_vertices() == points.size());
    REQUIRE(t.number_of_unbounded_faces() == 1);
    REQUIRE(t.number_of_faces() - 1 == points.size() - 2);

    for (auto fit = t.faces_begin(); fit != t.faces_end(); ++fit) {
        if (fit->is_unbounded())
            continue;
        auto vertices = face_vertices(fit);
        REQUIRE(vertices.size() == 3);
    }
}

TEST_CASE("Triangulate a polygon (200 vertices)") {
    std::string filename = "./instances/rand-200-66.pol";
    std::ifstream in(filename);
    REQUIRE(in);
    auto points = get_instance_from_file(in);
    auto t = triangulate<Arrangement_2>(points);
    REQUIRE(t.number_of_vertices() == points.size());
    REQUIRE(t.number_of_unbounded_faces() == 1);
    REQUIRE(t.number_of_faces() - 1 == points.size() - 2);

    for (auto fit = t.faces_begin(); fit != t.faces_end(); ++fit) {
        if (fit->is_unbounded())
            continue;
        auto vertices = face_vertices(fit);
        REQUIRE(vertices.size() == 3);
    }
}

TEST_CASE("Convex components (simple example) ") {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(6, 4),
        Kernel::Point_2(4, 4), Kernel::Point_2(2, 8), Kernel::Point_2(0, 4)};
    std::list<Partition_Polygon_2> output;
    to_convex_components(points, output);
    for (const auto &pol : output) {
        REQUIRE(pol.is_convex());
    }
}

TEST_CASE("Convex components (20 vertices) ") {
    std::string filename = "./instances/rand-20-1.pol";
    std::ifstream in(filename);
    REQUIRE(in);
    auto points = get_instance_from_file(in);
    std::list<Partition_Polygon_2> output;
    to_convex_components(points, output);
    for (const auto &pol : output) {
        REQUIRE(pol.is_convex());
    }
}

TEST_CASE("Convex components (100 vertices) ") {
    std::string filename = "./instances/rand-100-8.pol";
    std::ifstream in(filename);
    REQUIRE(in);
    auto points = get_instance_from_file(in);
    std::list<Partition_Polygon_2> output;
    to_convex_components(points, output);
    for (const auto &pol : output) {
        REQUIRE(pol.is_convex());
    }
}
