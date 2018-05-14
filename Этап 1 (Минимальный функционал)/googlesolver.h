#ifndef GOOGLESOLVER_H
#define GOOGLESOLVER_H
#define USE_GLOP
#include "ortools/base/commandlineflags.h"
#include "ortools/base/commandlineflags.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/linear_solver/linear_solver.pb.h"
#include <vector>
#include <milp.h>
using namespace std;
using namespace operations_research;
class googlesolver
{
public:
    googlesolver();
    double Solve(const vector<vector<ValueType>> &A, const vector<ValueType> &B,
               const vector<ValueType> &C, int N, vector<ValueType> &answers);
};

#endif // GOOGLESOLVER_H
