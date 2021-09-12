#include "../../../include/agp/cplex/cplex.h"

std::vector<int> IP_ind_solution(IloCplex &cplex, IloNumVarArray &x) {
    std::vector<int> s;
    for (IloInt i = 0; i < x.getSize(); ++i) {
        if (cplex.getValue(x[i]) > 0) {
            s.emplace_back((int)i);
        }
    }
    return s;
}

std::vector<IloNum> LP_solution(IloCplex &cplex, IloNumVarArray &x) {
    std::vector<IloNum> s;
    for (IloInt i = 0; i < x.getSize(); ++i) {
        s.push_back(cplex.getValue(x[i]));
    }
    return s;
}

std::vector<int> solve_IP_model(IloCplex &cplex, IloModel &model,
                                IloNumVarArray &x) {
    cplex.extract(model);
    cplex.solve();
    return IP_ind_solution(cplex, x);
}

std::vector<IloNum> solve_LP_model(IloCplex &cplex, IloModel &model,
                                   IloNumVarArray &x) {
    cplex.extract(model);
    cplex.solve();
    return LP_solution(cplex, x);
}
