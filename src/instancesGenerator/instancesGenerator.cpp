#include "instancesGenerator.hpp"
#include <cstdio>
#include <random>
#include <iostream>

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
    std::vector<int> professors = {1, 2, 3};
    std::vector<int> advisors;
  
    for (int i = 1; i <= numberOfProfessors; i++) {
        if (i <= numberOfSlots) {
            advisors.push_back(i);
        }
        if (i > 3) {
            professors.push_back(i);
        }
    }

    const int numberOfAdvisors = (int) advisors.size();

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
    findPermutations(numberOfSlots, numberOfAdvisors, 0, &currentPermutation, &permutations);
    for (auto permutation : permutations) {
        Instance instance(numberOfSlots, professors.size());
        int slot = 0;
        for (int i = 0; (size_t) i < permutation.size(); i++) {
            for (int j = 0; j < permutation[i]; j++) {
                instance.delegate(advisors[i], slot++);
            }
        }
        instances.push_back(instance);
    }
    
    return instances;
}

/* Number of slots = quantidade de trabalhos  */
std::vector<Instance> InstancesGenerator::generateRandomInstances(int numberOfSlots, int numberOfProfessors) {
    std::vector<Instance> instances;
    std::random_device rd;  // Criação de um gerador nao deterministico
    std::mt19937 gen(rd()); //Seed para o algoritmo de geração de numeros pseudoaleatorios Mersenne Twister
    int min, max;
    min = max = 1;
    
    /* Se a quantia de professores for maior ou igual a quantia de trabalhos, então o maximo de advisors disponiveis
     * para aleatoriedade será exatamente a quantia de trabalhos, caso contrario será a quantia de professores disponiveis*/
    max = (numberOfProfessors  >= numberOfSlots ? numberOfSlots: numberOfProfessors);
      
    std::uniform_int_distribution<> dist(min, max);
    std::vector < int > advisorsPerSlot; // Indica os orientadores para cada slot
    std::vector < int > countSlotPerAdvisor(max, 0); // Quantia de trabalhos orientados por cada professor
    int i = 0; 
    while(i < 1){
        
        /* Gera o id do advisor para cada trabalho aleatoriamente e incrementa em 1 a quantia  de trabalhos orientados por ele */
        for(int i = 0; i < numberOfSlots; i++){
          int advisorId = dist(gen);  
          advisorsPerSlot.push_back(advisorId);
          std::cout << advisorId << " ";
          countSlotPerAdvisor[advisorId] += 1; // Talvez nao precisamos fazer essa contagem          
        }
        i += 1;
      std::cout << "\n";
    getchar();
      
  }

    //throw "TODO: implement this method";

    return instances;
}
