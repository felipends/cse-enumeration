#include "instance.hpp"
#include <iostream>

Instance::Instance(int numberOfSlots, int numberOfProfessors) {
    this->professors = std::vector<int>(numberOfProfessors);
    for (int i = 1; i <= numberOfProfessors; i++) {
        this->professors.push_back(i);
    }

    this->advisorInSlot = std::vector<int>(numberOfSlots);
}

void Instance::delegate(int professor, int slot) {
    this->advisorInSlot[slot] = professor;
}

std::string Instance::toString() {
    std::string result = "";
    for (int i = 0; i < this->advisorInSlot.size(); i++) {
        result += std::to_string(this->advisorInSlot[i]) + " ";
    }
    
    return result;
}