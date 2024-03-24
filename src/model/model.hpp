#ifndef MODEL_HPP
#define MODEL_HPP

#include "../instance/instance.hpp"
#include <ortools/linear_solver/linear_solver.h>
#include <algorithm>

class Model {
    private:
        Instance instance;
        operations_research::MPSolver* model;

        std::vector<std::vector<std::vector<int>>> solution;

        void buildModel();
    public:
        Model(Instance i);
        int solve();
        
        std::vector<std::vector<std::vector<int>>> getSolution();
};

#endif