#include "googlesolver.h"
#include <milp.h>
#include "mainwindow.h"
googlesolver::googlesolver()
{

}

double googlesolver::Solve(const vector<vector<ValueType>> &A, const vector<ValueType> &B,
           const vector<ValueType> &C, const vector<int> &I, int N, vector<ValueType> &answers,
                           string solverName, bool isInt, int64 &time, int iterations) {
             MPSolver::OptimizationProblemType solverType;
             if (!isInt) {
                 if (solverName == "GLOP") {
                     solverType = MPSolver::GLOP_LINEAR_PROGRAMMING;
                 }
                 if (solverName == "CLP") {
                     solverType = MPSolver::CLP_LINEAR_PROGRAMMING;
                 }
                 if (solverName == "GLPK") {
                     solverType = MPSolver::GLPK_LINEAR_PROGRAMMING;
                 }
             } else {
                 if (solverName == "CBC") {
                     solverType = MPSolver::CBC_MIXED_INTEGER_PROGRAMMING;
                 }
                 if (solverName == "GLPK") {
                     solverType = MPSolver::GLPK_MIXED_INTEGER_PROGRAMMING;
                 }
             }

             MPSolver solver("solver", solverType);

             const double infinity = solver.infinity();
             vector<MPVariable*> X;
             for (int i = 0; i < N; ++i){
                 if(I[i] == 0) {
                     X.push_back(solver.MakeNumVar(-infinity, infinity, "x" + to_string(i+1)));
                 }
                 else if (I[i] == 1) {
                     X.push_back(solver.MakeIntVar(-infinity, infinity, "x" + to_string(i+1)));
                 }
             }
             MPObjective* const objective_function = solver.MutableObjective();
             for (int i = 0; i < N; ++i) {
                 objective_function->SetCoefficient(X[i],C[i]);
             }
             objective_function->SetMaximization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < B.size(); ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, B[i]));
                 for (int j = 0; j < N;++j) {
                     constraints[i]->SetCoefficient(X[j], A[i][j]);
                 }
             }
             MPSolver::ResultStatus result_status = solver.Solve();
             LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
             int64 totalTime = 0;
             totalTime += solver.wall_time();
             if (result_status != MPSolver::OPTIMAL) {
                 answers.clear();
                 return 0;
                 LOG(FATAL) << "The problem does not have an optimal solution!";
             }
             for (int i = 0; i <N; ++i) {
                 answers[i] = X[i]->solution_value();
             }
            double result_value = objective_function->Value();
            for (int i = 1; i < iterations; ++i) {
                solver.Reset();
                result_status = solver.Solve();
                totalTime += solver.wall_time();
                LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
            }
            totalTime /= iterations;
            time = totalTime;
            return result_value;
}
