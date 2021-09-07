#include <agp/algo/baumgartner_algorithm.h>
#include <agp/arrangement/partition.h>

using vec_t = std::vector<Kernel::Point_2>;

void visibility_matrix (const wg_placement &wg, IloEnv &env, IloArray<IloNumArray> &A) {
    IloInt w_size = wg.witnesses.size();
    IloInt g_size = wg.guards.size();
    for (int i = 0; i < w_size; ++i) {
        A[i] = IloNumArray(env, g_size);
        for (int j = 0; j < g_size; ++j) {
            A[i][j] = point_visible(wg.guard_visibility[j], wg.witnesses[i]);
        }
    }
}

void guards_model (IloModel &g_model, IloNumVarArray &x, IloArray<IloNumArray> &A, IloEnv &env) {
    auto w_size = A.getSize();
    for (int i = 0; i < w_size; ++i) {
        g_model.add(IloScalProd(A[i], x) >= 1);
    }
    g_model.add(IloMinimize(env, IloSum(x)));
}

void witnesses_model (IloModel &w_model, IloNumVarArray &y, IloArray<IloNumArray> &A, IloEnv &env) {
    auto w_size = A.getSize();
    auto g_size = A[0].getSize();
    for (int j = 0; j < g_size; ++j) {
        IloNumArray row(env, w_size);
        for (int i = 0; i < w_size; ++i) {
            row[i] = A[i][j];
        }
        w_model.add(IloScalProd(row, y) <= 1);
    }
    w_model.add(IloMaximize(env, IloSum(y)));
}

bool integral_solution (const std::vector<IloNum> &sol) {
    for (auto g: sol) {
        if(g > 0 && g < 1)
            return false;
    }
    return true;
}

Arrangement_2 overlay_arrangements (const Arrangement_2 &primary, const std::vector<Arrangement_2> &arrangements, const std::vector<int> &chosen) {
    Arrangement_2 arrangement = primary;
    for (const auto& i: chosen) {
        Arrangement_2 arr_output;
        overlay(arrangement, arrangements[i], arr_output);
        arrangement = arr_output;
    }
    return arrangement;
}

vec_t primary_separation (const Arrangement_2 &guards_overlay, const std::vector<int> &curr_g, const std::vector<IloNum> &x, const std::vector<Arrangement_2> &guard_visibility) {
    vec_t found_witnesses;
    auto points = arr_discretization(guards_overlay);
    for(const auto &c: points) {
        IloNum sum = 0;
        for (unsigned i = 0; i < curr_g.size() && sum < 1; ++i) {
            auto ind = curr_g[i];
            sum += x[ind] * point_visible(guard_visibility[ind], c);
        }
        if (sum < 1) found_witnesses.push_back(c);
    }
    return found_witnesses;
}

vec_t dual_separation (const Arrangement_2 &witnesses_overlay, const std::vector<int> &curr_w, const std::vector<IloNum> &y, const std::vector<Arrangement_2> &witness_visibility) {
    vec_t found_guards;
    auto points = arr_discretization(witnesses_overlay);
    for(const auto &c: points) {
        if (!found_guards.empty()) break;
        IloNum guards = 0;
        for (unsigned i = 0; guards <= 1 && i < curr_w.size(); ++i) {
            auto ind = curr_w[i];
            guards += y[ind] * point_visible(witness_visibility[ind], c);
        }
        if (guards > 1) found_guards.push_back(c);
    }
    return found_guards;
}

std::vector<int> LP_chosen (std::vector<IloNum> &solution) {
    std::vector<int> v;
    for (unsigned i = 0; i < solution.size(); ++i)
        if (solution[i] > 0) v.push_back(i);
    return v;
}

vec_t baumgartner_algorithm (const vec_t &points, std::function<wg_placement(const vec_t &)> initial_placement_method, int sec) {
    clock_t begin = clock(), end;
    auto polygon = create_arrangement<Arrangement_2>(points);
    wg_placement wg = initial_placement_method(points);

    IloEnv env;
    IloInt w_size = wg.witnesses.size(), g_size;
    IloArray<IloNumArray> A(env, w_size);

    visibility_matrix(wg, env, A);

    IloCplex cplex(env);
    cplex.setOut(env.getNullStream());

    bool witness_found, guard_found;
    unsigned long lower_bound = 0, upper_bound = points.size() / 3;
    std::vector<int> g_chosen, w_chosen;
    std::vector<IloNum> x_solved, y_solved;
    unsigned long current_best_size = points.size() / 3 + 1;
    std::vector<int> current_best;
    do {
        witness_found = false;
        guard_found = false;
        w_size = wg.witnesses.size();
        g_size = wg.guards.size();

        IloNumVarArray x(env, g_size, 0, 1);
        IloModel g_model(env);
        IloNumVarArray y(env, w_size, 0, 1);
        IloModel w_model(env);

        guards_model(g_model, x, A, env);

        witnesses_model(w_model, y, A, env);

        x_solved = solve_LP_model(cplex, g_model, x);
        y_solved = solve_LP_model(cplex, w_model, y);

        g_chosen = LP_chosen(x_solved);
        w_chosen = LP_chosen(y_solved);

        Arrangement_2 guards_arrangement = overlay_arrangements(polygon, wg.guard_visibility, g_chosen);
        Arrangement_2 witnesses_arrangement = overlay_arrangements(polygon, wg.witness_visibility, w_chosen);

        auto W = primary_separation(guards_arrangement, g_chosen, x_solved, wg.guard_visibility);
        for (const auto &w: W) {
            witness_found = true;
            wg.witnesses.push_back(w);
            wg.witness_visibility.push_back(general_point_visibility_region(polygon, w));
            A.add(IloNumArray(env, g_size));
            int n = static_cast<int>(A.getSize() - 1);
            for (int j = 0; j < g_size; ++j) {
                A[n][j] = point_visible(wg.guard_visibility[j], w);
            }
        }

        if (!witness_found && current_best_size > g_chosen.size()) {
            current_best = g_chosen;
            current_best_size = g_chosen.size();
        }

        if (!witness_found && integral_solution(x_solved) && upper_bound > g_chosen.size())
            upper_bound = g_chosen.size();

        auto G = dual_separation(witnesses_arrangement, w_chosen, y_solved, wg.witness_visibility);
        for (const auto &g: G) {
            guard_found = true;
            wg.guards.push_back(g);
            wg.guard_visibility.push_back(general_point_visibility_region(polygon, g));
            unsigned long n = wg.guards.size() - 1;
            for (int i = 0; i < A.getSize(); ++i) {
                A[i].add(point_visible(wg.guard_visibility[n], wg.witnesses[i]));
            }
        }
        if (!guard_found && lower_bound < g_chosen.size()) lower_bound = g_chosen.size();

        w_model.end();
        g_model.end();

        end = clock();

    } while ((witness_found || guard_found) && lower_bound < upper_bound && double(end - begin) / CLOCKS_PER_SEC < sec);

    vec_t solution;
    for(auto i: current_best) {
        solution.push_back(wg.guards[i]);
    }
    return solution;
}

