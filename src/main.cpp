#include <iostream>
#include <fstream>
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

    //initialize json file to write solutions
    std::fstream jsonFile;
    jsonFile.open("solution.json", std::ios::out);
    jsonFile << "{\n";

    for (int i = 1; i <= 3*size; i++) {
        jsonFile << "\"" << i << "_professors\": [\n";
        std::cout << "Instances for " << i << " professors" << std::endl;
        std::vector<Instance> instances = InstancesGenerator::generateInstances(size, i);
        int instanceIndex = 0;
        for (auto instance : instances) {
            Model model = Model(instance);
            int solutionValue = model.solve();

            //write solution to json
            if (instanceIndex == instances.size() - 1) {
                jsonFile << model.getSolutionAsJSON() << std::endl;
            } else {
                jsonFile << model.getSolutionAsJSON() << "," << std::endl;
            }
            instanceIndex++;
        }

        std::cout << "Done" << std::endl;

        if (i == 3*size) {
            jsonFile << "]\n";
        } else {
            jsonFile << "],\n";
        }
    }

    jsonFile << "}\n";
    jsonFile.close();

    return 0;
}