#include "ortools/base/commandlineflags.h"
#include "ortools/base/commandlineflags.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/linear_solver/linear_solver.pb.h"
#include <vector>
#include <string>
using namespace std;
/*
AX <= B
CX - > MAX
Возвращает оптимальное значение. Коэффициенты в векторе answers.
*/

//make rcc EX=linear_programming_solver.cc
namespace operations_research {
double Solve(const vector<vector<int>> &A, const vector<int> &B,
           const vector<int> &C, int N, vector<double> &answers) {
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
             //objective_function->SetMinimization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < N; ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, B[i]));
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
                 answers[i] = X[i]->solution_value();
             }
            return objective_function->Value();
}

//целочисленное программирование
double SolveInteger(const vector<vector<int>> &A, const vector<int> &B,
           const vector<int> &C, int N, vector<double> &answers) {
             MPSolver solver("solver", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
             const double infinity = solver.infinity();
             vector<MPVariable*> X;
             for (int i = 0; i < N; ++i){
                 X.push_back(solver.MakeIntVar(-infinity, infinity, "x" + to_string(i+1)));
             }
             MPObjective* const objective_function = solver.MutableObjective();
             for (int i = 0; i < N; ++i) {
                 objective_function->SetCoefficient(X[i],C[i]);
             }
             objective_function->SetMaximization();
             //objective_function->SetMinimization();
             vector<MPConstraint*> constraints;
             for (int i = 0; i < N; ++i) {
                 constraints.push_back(solver.MakeRowConstraint(-infinity, B[i]));
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
             answers[i] = X[i]->solution_value();
            }

            return objective_function->Value();
}
}

int main() {
  int N = 3;
  vector<vector<int>> A(N);
  for (int i =0; i < N; ++i) {
      A[i].resize(N);
  }
  A[0][0] = 1;
  A[0][1] = 1;
  A[0][2] = 1;
  A[1][0] = 10;
  A[1][1] = 4;
  A[1][2] = 5;
  A[2][0] = 2;
  A[2][1] = 2;
  A[2][2] = 6;
  vector<int> B = {100,600,300};
  vector<int> C = {10,6,4};
  vector<double> result(N);
  double opt = operations_research::Solve(A,B,C,N,result);
  cout << opt << endl;
  for (int i = 0; i < N;++i) {
      cout << result[i] << " ";
  }
}
