#include <agp/arrangement/utils.h>

#include <catch2/catch.hpp>

TEST_CASE("Create arrangement from a vector iterator") {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(5, 2),
        Kernel::Point_2(4, 4), Kernel::Point_2(2, 8), Kernel::Point_2(0, 4)};
    auto arr = create_arrangement<Arrangement_2>(points.begin(), points.end());
    REQUIRE(arr.number_of_faces() == 2);
    REQUIRE(arr.number_of_unbounded_faces() == 1);
    REQUIRE(arr.number_of_vertices() == 6);
    auto fit = arr.faces_begin();
    while (fit->is_unbounded())
        fit++;
    auto circ = fit->outer_ccb(), curr = circ;
    while (curr->source()->point() != points[0]) {
        curr++;
    }
    circ = curr;
    unsigned long i = 0;
    do {
        auto next = (i + 1) % points.size();
        REQUIRE(curr->source()->point() == points[i]);
        REQUIRE(curr->target()->point() == points[next]);
        ++i;
    } while (++curr != circ);
}

TEST_CASE("Create arrangement from a vector") {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(5, 2),
        Kernel::Point_2(4, 4), Kernel::Point_2(2, 8), Kernel::Point_2(0, 4)};
    auto arr = create_arrangement<Arrangement_2>(points);
    REQUIRE(arr.number_of_unbounded_faces() == 1);
    REQUIRE(arr.number_of_faces() == 2);
    REQUIRE(arr.number_of_edges() == 6);
    REQUIRE(arr.number_of_vertices() == 6);
    auto fit = arr.faces_begin();
    while (fit->is_unbounded())
        fit++;
    auto circ = fit->outer_ccb(), curr = circ;
    while (curr->source()->point() != points[0]) {
        curr++;
    }
    circ = curr;
    unsigned long i = 0;
    do {
        auto next = (i + 1) % points.size();
        REQUIRE(curr->source()->point() == points[i++]);
        REQUIRE(curr->target()->point() == points[next]);
    } while (++curr != circ);
}

TEST_CASE("Face vertices") {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(5, 2),
        Kernel::Point_2(4, 4), Kernel::Point_2(2, 8), Kernel::Point_2(0, 4)};
    auto arr = create_arrangement<Arrangement_2>(points);
    auto fit = arr.faces_begin();
    while (fit->is_unbounded())
        fit++;
    auto v = face_vertices(fit);
    REQUIRE(v.size() == points.size());
}

TEST_CASE("Overlay arrangements case with one arrangement inside another, "
          "partly sharing sides") {
    std::vector<Kernel::Point_2> points1 = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(4, 4),
        Kernel::Point_2(0, 4)};
    std::vector<Kernel::Point_2> points2 = {
        Kernel::Point_2(0, 0), Kernel::Point_2(2, 0), Kernel::Point_2(2, 2),
        Kernel::Point_2(0, 2)};
    auto arr1 = create_arrangement<Arrangement_2>(points1),
         arr2 = create_arrangement<Arrangement_2>(points2);
    std::vector<Arrangement_2> v_a = {arr1, arr2};
    auto overlay_arr =
        overlay_arrangements<Arrangement_2>(Arrangement_2(), v_a);
    REQUIRE(overlay_arr.number_of_faces() == 3);
    REQUIRE(overlay_arr.number_of_edges() == 8);
    for (auto vit = overlay_arr.vertices_begin();
         vit != overlay_arr.vertices_end(); ++vit) {
        if (vit->point() == Kernel::Point_2(0, 2) ||
            vit->point() == Kernel::Point_2(2, 0)) {
            REQUIRE(vit->degree() == 3);
        } else
            REQUIRE(vit->degree() == 2);
    }
}

TEST_CASE("Overlay not overlaying arrangement") {
    std::vector<Kernel::Point_2> points1 = {
        Kernel::Point_2(0, 0), Kernel::Point_2(4, 0), Kernel::Point_2(4, 4),
        Kernel::Point_2(0, 4)};
    std::vector<Kernel::Point_2> points2 = {
        Kernel::Point_2(6, 0), Kernel::Point_2(8, 0), Kernel::Point_2(8, 2),
        Kernel::Point_2(6, 2)};
    auto arr1 = create_arrangement<Arrangement_2>(points1),
         arr2 = create_arrangement<Arrangement_2>(points2);
    std::vector<Arrangement_2> v_a = {arr1, arr2};
    auto overlay_arr = overlay_arrangements(Arrangement_2(), v_a);
    REQUIRE(overlay_arr.number_of_faces() == 3);
    REQUIRE(overlay_arr.number_of_edges() == 8);
    for (auto vit = overlay_arr.vertices_begin();
         vit != overlay_arr.vertices_end(); ++vit) {
        REQUIRE(vit->degree() == 2);
    }
}
