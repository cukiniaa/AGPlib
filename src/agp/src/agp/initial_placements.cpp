#include <agp/initial_placements.h>

wg_placement every_second(const std::vector<Kernel::Point_2> &points) {
    wg_placement wg;
    Arrangement_2 arr_polygon = create_arrangement<Arrangement_2>(points);
    std::vector<Arrangement_2> visibility_regions =
        all_visibility_regions(arr_polygon, points);
    unsigned i = 0, n = points.size() - 1;
    while (i < n) {
        wg.guards.push_back(points[i]);
        wg.guard_visibility.push_back(visibility_regions[i]);
        wg.witnesses.push_back(points[i + 1]);
        wg.witness_visibility.push_back(visibility_regions[i + 1]);
        i = i + 2;
    }

    if (n % 2 == 0) {
        wg.witnesses.push_back(points[n]);
        wg.witness_visibility.push_back(visibility_regions[n]);
    }

    return wg;
}
