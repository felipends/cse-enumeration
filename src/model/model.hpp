#ifndef MODEL_HPP
#define MODEL_HPP

#include "../instance/instance.hpp"
#include <ortools/linear_solver/linear_solver.h>
#include <algorithm>

class Model {
    private:
        Instance instance;
        operations_research::MPSolver* model;

        void buildModel(int hmaxValue = 0, bool useHmax = false);
    public:
        Model(Instance i);
        int solve();
        void rebuildModel(int hmaxValue);
        void printSolution();
};

#endif