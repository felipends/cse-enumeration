#include <iostream>
#include <string>
#include <vector>
#include "instance/instance.hpp"
#include "model/model.hpp"

void findPermutations(int numberOfSlots, int numberOfProfessors, int currentValue, std::vector<int>* currentPermutation, std::vector<std::vector<int>>* permutations) {
    if (numberOfProfessors == 0 && currentValue == numberOfSlots) {
        permutations->push_back(*currentPermutation);
        return;
    }

    if (numberOfProfessors == 0) {
        return;
    }

    for (int i = 1; i <= numberOfSlots; i++) {
        currentPermutation->push_back(i);
        findPermutations(numberOfSlots, numberOfProfessors - 1, currentValue + i, currentPermutation, permutations);
        currentPermutation->pop_back();
    }
}

std::vector<Instance> generateInstances(int numberOfSlots, int numberOfProfessors) {
    std::vector<Instance> instances;
    std::vector<int> professors(numberOfProfessors);
    for (int i = 1; i <= numberOfProfessors; i++) {
        professors[i-1] = i;
    }

    if (numberOfSlots == numberOfProfessors) {
        Instance instance(numberOfSlots, numberOfProfessors);
        for (int i = 0; i < numberOfSlots; i++) {
            instance.delegate(professors[i], i);
        }
        instances.push_back(instance);
        return instances;
    }

    std::vector<std::vector<int>> permutations;
    std::vector<int> currentPermutation;
    findPermutations(numberOfSlots, numberOfProfessors, 0, &currentPermutation, &permutations);
    for (auto permutation : permutations) {
        Instance instance(numberOfSlots, numberOfProfessors);
        int slot = 0;
        for (int i = 0; i < permutation.size(); i++) {
            for (int j = 0; j < permutation[i]; j++) {
                instance.delegate(professors[i], slot++);
            }
        }
        instances.push_back(instance);                
    }
    
    return instances;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <size of session>" << std::endl;
        return 1;
    }

    if (argv[1][0] < '0' || argv[1][0] > '9' ) {
        std::cerr << "Error: " << argv[1] << " is not a number" << std::endl;
        return 1;
    }
    
    int size = std::stoi(argv[1]);
    if (size < 3) {
        std::cerr << "Error: " << size << " is not a valid size" << std::endl;
        return 1;
    }

    for (int i = 3; i <= size; i++) {
        std::cout << "Instances for " << i << " professors" << std::endl;
        std::vector<Instance> instances = generateInstances(size, i);
        for (auto instance : instances) {
            Model model = Model(instance);
            while (true) {
                int solutionValue = model.solve();
                std::cout << solutionValue << std::endl;
                model.printSolution();
                if (solutionValue > 0) {
                    model.rebuildModel(solutionValue);
                    continue;
                }
                break;
            }
        }
    }

    return 0;
}