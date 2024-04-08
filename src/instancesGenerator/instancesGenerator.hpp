#ifndef INSTANCES_GENERATOR_HPP
#define INSTANCES_GENERATOR_HPP

#include <vector>
#include "../instance/instance.hpp"

class InstancesGenerator {
    private:
        static void findPermutations(int numberOfSlots, int numberOfProfessors, int currentValue, std::vector<int>* currentPermutation, std::vector<std::vector<int>>* permutations);
    public:
        InstancesGenerator();
        static std::vector<Instance> generateInstances(int numberOfSlots, int numberOfProfessors);
        static std::vector<Instance> generateRandomInstances(int numberOfSlots, int numberOfProfessors);
};

#endif