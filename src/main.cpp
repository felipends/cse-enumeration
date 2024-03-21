#include <iostream>
#include <string>
#include <vector>
#include "instance/instance.hpp"

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

    int sizeOfSubset = numberOfSlots - numberOfProfessors + 1;
    std::vector<std::vector<int>> initialSubsets;
    for (int i = 0; i < numberOfProfessors; i++) {
        std::vector<int> subset;
        for (int j = 0; j < sizeOfSubset; j++) {
            subset.push_back(i+1);
        }
        initialSubsets.push_back(subset);
        sizeOfSubset = 1;
    }

    Instance instance(numberOfSlots, numberOfProfessors);
    int slot = 0;
    for (int i = 0; i < initialSubsets.size(); i++) {
        for (int j = 0; j < initialSubsets[i].size(); j++) {
            instance.delegate(initialSubsets[i][j], slot);
            slot++;
        }
    }

    instances.push_back(instance);

    //TODO: generalize for all subsets grow and shrink to follow the pattern
    int shrinkingIndex = 0;
    int increasingIndex = 1;
    while(true) {
        std::vector<std::vector<int>> subsets;
        for (int i = 0; i < initialSubsets.size(); i++) {
            std::vector<int> subset = initialSubsets[i];
            if (i == shrinkingIndex && subset.size() > 1) {
                subset.pop_back();
            } else if (i == increasingIndex && subset.size() < numberOfSlots - numberOfProfessors + 1) {
                subset.push_back(*subset.begin());
            }

            subsets.push_back(subset);
        }

        initialSubsets = subsets;

        instance = Instance(numberOfSlots, numberOfProfessors);
        slot = 0;
        for (int i = 0; i < subsets.size(); i++) {
            for (int j = 0; j < subsets[i].size(); j++) {
                instance.delegate(subsets[i][j], slot);
                slot++;
            }
        }

        instances.push_back(instance);

        if (initialSubsets[shrinkingIndex].size() == 1) {
            shrinkingIndex++;
        }

        if (initialSubsets[increasingIndex].size() == numberOfSlots - numberOfProfessors + 1) {
            increasingIndex++;
        }

        if (shrinkingIndex == initialSubsets.size() - 1) {
            break;
        }
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
            std::cout << instance.toString() << std::endl;
        }
    }

    return 0;
}