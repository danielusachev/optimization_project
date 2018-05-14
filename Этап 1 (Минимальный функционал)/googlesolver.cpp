#include "googlesolver.h"
#include <milp.h>
googlesolver::googlesolver()
{

}

double googlesolver::Solve(const vector<vector<ValueType>> &A, const vector<ValueType> &B,
           const vector<ValueType> &C, int N, vector<ValueType> &answers) {
             MPSolver solver("solver", MPSolver::GLOP_LINEAR_PROGRAMMING);
             const double infinity = solver.infinity();
             vector<MPVariable*> X;
             for (int i = 0; i < N; ++i){
                 X.push_back(solver.MakeNumVar(0.0, infinity, "x" + to_string(i+1)));
             }
             MPObjective* const objective_function = solver.MutableObjective();
             for (int i = 0; i < N; ++i) {
                 objective_function->SetCoefficient(X[i],C[i]);
             }
             objective_function->SetMaximization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < N; ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, B[i]));
                 for (int j = 0; j < N;++j) {
                     constraints[i]->SetCoefficient(X[j], A[i][j]);
                 }
             }
             const MPSolver::ResultStatus result_status = solver.Solve();
             //LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
             if (result_status != MPSolver::OPTIMAL) {
                 //LOG(FATAL) << "The problem does not have an optimal solution!";
             }
             for (int i = 0; i <N; ++i) {
                 answers[i] = X[i]->solution_value();
             }
            return objective_function->Value();
}
