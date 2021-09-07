#include <catch2/catch.hpp>
#include <agp/arrangement/utils.h>
#include <agp/arrangement/visibility.h>

TEST_CASE ( "Point visibility (vertex)" ) {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0),
		Kernel::Point_2(4, 0),
		Kernel::Point_2(6, 4),
		Kernel::Point_2(4, 4),
		Kernel::Point_2(2, 8),
		Kernel::Point_2(0, 4)
    };
    auto arr = create_arrangement<Arrangement_2>(points);
    auto vis = general_point_visibility_region(arr, Kernel::Point_2(6, 4));
    REQUIRE(vis.number_of_faces() == 2);
    REQUIRE(vis.number_of_vertices() == 5);

    std::vector<Kernel::Point_2> vis_points = {
        Kernel::Point_2(6, 4),
		Kernel::Point_2(4, 4),
		Kernel::Point_2(0, 4),
		Kernel::Point_2(0, 0),
		Kernel::Point_2(4, 0)
	};
    auto fit = vis.faces_begin();
    while(fit->is_unbounded()) fit++;
    auto circ = fit->outer_ccb(), curr = circ;
    while(curr->source()->point() != vis_points[0]) { curr++; }
    circ = curr;
    unsigned long i = 0;
    do {
        auto next = (i + 1) % vis_points.size();
        REQUIRE(curr->source()->point() == vis_points[i]);
        REQUIRE(curr->target()->point() == vis_points[next]);
        ++i;
    } while (++curr != circ);
}

TEST_CASE ( "Point visibility (point inside)" ) {
    std::vector<Kernel::Point_2> points = {
        Kernel::Point_2(0, 0),
		Kernel::Point_2(4, 0),
		Kernel::Point_2(6, 4),
		Kernel::Point_2(4, 4),
		Kernel::Point_2(2, 8),
		Kernel::Point_2(0, 4)
	};
    auto arr = create_arrangement<Arrangement_2>(points);
    auto vis = general_point_visibility_region(arr, Kernel::Point_2(1, 1));
    REQUIRE(vis.number_of_faces() == 2);
    REQUIRE(vis.number_of_vertices() == 6);

    auto fit = vis.faces_begin();
    while(fit->is_unbounded()) fit++;
    auto circ = fit->outer_ccb(), curr = circ;
    while(curr->source()->point() != points[0]) { curr++; }
    circ = curr;
    unsigned long i = 0;
    do {
        auto next = (i + 1) % points.size();
        REQUIRE(curr->source()->point() == points[i]);
        REQUIRE(curr->target()->point() == points[next]);
        ++i;
    } while (++curr != circ);

}


TEST_CASE ( "Point visible" ) {
    std::vector<Kernel::Point_2> points = { Kernel::Point_2(0, 0),
		Kernel::Point_2(4, 0),
		Kernel::Point_2(6, 4),
		Kernel::Point_2(4, 4),
		Kernel::Point_2(2, 8),
		Kernel::Point_2(0, 4)
	};
    auto arr = create_arrangement<Arrangement_2>(points);
    auto vis = general_point_visibility_region(arr, Kernel::Point_2(6, 4));

    REQUIRE(point_visible(vis, Kernel::Point_2(0, 0)));
    REQUIRE(point_visible(vis, Kernel::Point_2(1, 1)));
    REQUIRE(point_visible(vis, Kernel::Point_2(0, 2)));
    REQUIRE(!point_visible(vis, Kernel::Point_2(2, 5)));
    REQUIRE(!point_visible(vis, Kernel::Point_2(6, 2)));
    REQUIRE(!point_visible(vis, Kernel::Point_2(-1, 2)));
}
