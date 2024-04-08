#include <iostream>
#include <string>
#include <vector>
#include "instance/instance.hpp"
#include "model/model.hpp"
#include "instancesGenerator/instancesGenerator.hpp"

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

    for (int i = 1; i <= 3*size; i++) {
        std::cout << "Instances for " << i << " professors" << std::endl;
        std::vector<Instance> instances = InstancesGenerator::generateInstances(size, i);
        for (auto instance : instances) {
            std::cout << "Instance: " << instance.toString() << std::endl;
            Model model = Model(instance);
            int solutionValue = model.solve();
            std::cout << solutionValue << std::endl;
            model.printSolution();
        }
    }

    return 0;
}