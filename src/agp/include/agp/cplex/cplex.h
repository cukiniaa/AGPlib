/*! \file cplex.h
 * \brief A wrap for the CPLEX package.
 */
#ifndef AGP_CPLEX_H
#define AGP_CPLEX_H

#include <vector>
#include <ilcplex/ilocplex.h>

/*! \fn std::vector<int> IP_ind_solution (IloCplex &cplex, IloNumVarArray &x)
 * \brief Returns indices of elements from solved vector where elements are greater than 0.
 * @param cplex - object of the IloCplex class
 * @param x - solved vector of decision variables
 * @return vector of indices of elements that are greater than 0
 */
std::vector<int> IP_ind_solution(IloCplex &cplex, IloNumVarArray &x);

/*! \fn std::vector<IloNum> LP_solution (IloCplex &cplex, IloNumVarArray &x)
 * \brief Coverts a solution of linear programming to a vector.
 * @param cplex - object of the IloCplex class
 * @param x - solved vector of decision variables
 * @return vector of values forming the solution
 */
std::vector<IloNum> LP_solution (IloCplex &cplex, IloNumVarArray &x);

/*! \fn std::vector<int> solve_IP_model (IloCplex &cplex, IloModel &model, IloNumVarArray &x)
 * \brief Solves a model of integer programming and returns the solution as indices of elements of x greater than 0.
 * @param cplex - object of the IloCplex class
 * @param model - model to be solved
 * @param x - vector of decision variables
 * @return vector of indices of elements that are greater than 0
 */
std::vector<int> solve_IP_model (IloCplex &cplex, IloModel &model, IloNumVarArray &x);

/*! \fn std::vector<IloNum> solve_LP_model (IloCplex &cplex, IloModel &model, IloNumVarArray &x)
 * \brief Solves a model of linear programming and returns the solution as a vector of all values of x.
 * @param cplex - object of the IloCplex class
 * @param model - model to be solved
 * @param x - vector of decision variables
 * @return vector of values forming the solution
 */
std::vector<IloNum> solve_LP_model (IloCplex &cplex, IloModel &model, IloNumVarArray &x);

#endif //AGP_CPLEX_H
