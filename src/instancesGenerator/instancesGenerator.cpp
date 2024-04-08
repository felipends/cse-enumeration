#include "instancesGenerator.hpp"

InstancesGenerator::InstancesGenerator() {}

void InstancesGenerator::findPermutations(int numberOfSlots, int numberOfProfessors, int currentValue, std::vector<int>* currentPermutation, std::vector<std::vector<int>>* permutations) {
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

std::vector<Instance> InstancesGenerator::generateInstances(int numberOfSlots, int numberOfProfessors) {
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
        for (int i = 0; (size_t) i < permutation.size(); i++) {
            for (int j = 0; j < permutation[i]; j++) {
                instance.delegate(professors[i], slot++);
            }
        }
        instances.push_back(instance);
    }
    
    return instances;
}

std::vector<Instance> InstancesGenerator::generateRandomInstances(int numberOfSlots, int numberOfProfessors) {
    std::vector<Instance> instances;
    
    throw "TODO: implement this method";

    return instances;
}