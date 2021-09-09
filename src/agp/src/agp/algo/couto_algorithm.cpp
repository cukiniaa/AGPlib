#include <agp/algo/couto_algorithm.h>
#include <agp/plot/agp_plot.h>

using vec_t = std::vector<Kernel::Point_2>;

std::vector<Polygon_with_holes_2> uncovered_regions (
        Polygon_2 &polygon,
        std::vector<Polygon_2> visibility_polygons,
        std::vector<int> guards);

void create_model (
        const vec_t &discretization,
        const std::vector<Arrangement_2> &visibility_regions,
        IloEnv &env,
        IloModel &model,
        IloNumVarArray &x);

vec_t couto_algorithm(const vec_t &points, const std::function<vec_t(const vec_t &)>& discretization_method) {
    auto arr_polygon = create_arrangement<Arrangement_2>(points);
    Polygon_2 polygon = create_polygon(points);

    std::vector<Arrangement_2> visibility_regions = all_visibility_regions(arr_polygon, points);
    std::vector<Polygon_2> visibility_polygons = arrangements_to_polygons(visibility_regions);
    vec_t discretization = discretization_method(points);

    IloEnv env;
    IloInt n = points.size();
    IloNumVarArray x(env, n, 0, 1, ILOINT);
    IloModel model(env);

    create_model(discretization, visibility_regions, env, model, x);

    IloCplex cplex(env);
    cplex.setOut(env.getNullStream());

    bool regions_to_cover;
    std::vector<int> guards;

    do {
        cplex.extract(model);
        cplex.solve();
        guards = IP_ind_solution(cplex, x);

        std::vector<Polygon_with_holes_2> ur = uncovered_regions(polygon, visibility_polygons, guards);
        regions_to_cover = !ur.empty();
        for (const auto &r: ur) {
            auto arr = create_arrangement<Arrangement_2>(r.outer_boundary().vertices_begin(), r.outer_boundary().vertices_end());
            auto to_add = arr_discretization(arr);

            for(const auto &c: to_add) {
                IloIntArray new_row(env, n);
                for (int j = 0; j < n; ++j) {
                    bool a = point_visible(visibility_regions[j], c);
                    new_row[j] = a;
                }
                model.add(IloScalProd(new_row, x) >= 1);
            }
        }
    } while (regions_to_cover);

    vec_t final_solution;
    for (const auto i: guards) {
        final_solution.push_back(points[i]);
    }
    return final_solution;
}

std::vector<Polygon_with_holes_2> uncovered_regions (Polygon_2 &polygon, std::vector<Polygon_2> visibility_polygons, std::vector<int> guards) {
    std::vector<Polygon_with_holes_2> result;
    Polygon_2 empty;
    CGAL::difference(polygon, empty, std::back_inserter(result));

    for (auto const i: guards) {
        std::vector<Polygon_with_holes_2> tmp;
        for (const auto &result_polygon: result)
            CGAL::difference(result_polygon, visibility_polygons[i], std::back_inserter(tmp));
        result = tmp;
    }
    return result;
}

void create_model (const vec_t &discretization, const std::vector<Arrangement_2> &visibility_regions, IloEnv &env, IloModel &model, IloNumVarArray &x) {
    IloInt n = visibility_regions.size();
    IloInt m = discretization.size();
    for (IloInt i = 0; i < m; ++i) {
        IloIntArray row(env, n);
        for (int j = 0; j < n; ++j) {
            row[j] = point_visible(visibility_regions[j], discretization[i]);
        }
        model.add(IloScalProd(row, x) >= 1);
    }

    model.add(IloMinimize(env, IloSum(x)));
}
