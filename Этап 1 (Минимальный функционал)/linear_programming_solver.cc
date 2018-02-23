#include "ortools/base/commandlineflags.h"
#include "ortools/base/commandlineflags.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/linear_solver/linear_solver.pb.h"
#include <vector>
#include <string>
using namespace std;
/*
AX <= C
BX - > MAX
Возвращает оптимальное значение. Коэффициенты в векторе answers.
*/

//make rcc EX=examples/cpp/file.cc

double Solve(const vector<vector<int>> &A, const vector<int> &C,
           const vector<int> &B, int N, vector<double> &answers) {
             MPSolver solver("solver", MPSolver::GLOP_LINEAR_PROGRAMMING);
             const double infinity = solver.infinity();
             vector<MPVariable*> X;
             for (int i = 0; i < N; ++i){
                 X.push_back(solver.MakeNumVar(-infinity, infinity, "x" + to_string(i+1)));
             }
             MPObjective* const objective_function = solver.MutableObjective();
             for (int i = 0; i < N; ++i) {
                 objective_function->SetCoefficient(X[i],B[i]);
             }
             objective_function->SetMaximization();
             //objective_function->SetMinimization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < N; ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, C[i]));
                 for (int j = 0; j < N;++j) {
                     constraints[i]->SetCoefficient(X[j], A[i][j]);
                 }
             }
             const MPSolver::ResultStatus result_status = solver.Solve();
             LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
             if (result_status != MPSolver::OPTIMAL) {
               LOG(FATAL) << "The problem does not have an optimal solution!";
             }
             for (int i = 0; i <N; ++i) {
             answers[i] = X[i]->solution_value()
            }
            return objective_function->Value();
}

//целочисленное программирование
double SolveInteger(const vector<vector<int>> &A, const vector<int> &C,
           const vector<int> &B, int N, vector<double> &answers) {
             MPSolver solver("solver", MPSolver::GLPK_MIXED_INTEGER_PROGRAMMING);
             const double infinity = solver.infinity();
             vector<MPVariable*> X;
             for (int i = 0; i < N; ++i){
                 X.push_back(solver.MakeIntVar(-infinity, infinity, "x" + to_string(i+1)));
             }
             MPObjective* const objective_function = solver.MutableObjective();
             for (int i = 0; i < N; ++i) {
                 objective_function->SetCoefficient(X[i],B[i]);
             }
             objective_function->SetMaximization();
             //objective_function->SetMinimization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < N; ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, C[i]));
                 for (int j = 0; j < N;++j) {
                     constraints[i]->SetCoefficient(X[j], A[i][j]);
                 }
             }
             const MPSolver::ResultStatus result_status = solver.Solve();
             LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
             if (result_status != MPSolver::OPTIMAL) {
               LOG(FATAL) << "The problem does not have an optimal solution!";
             }
             for (int i = 0; i <N; ++i) {
             answers[i] = X[i]->solution_value()
            }

            return objective_function->Value();
}
