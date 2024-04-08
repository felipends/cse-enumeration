#include "instance.hpp"
#include <iostream>

Instance::Instance(int numberOfSlots, int numberOfProfessors): numberOfSlots(numberOfSlots), numberOfProfessors(numberOfProfessors) {
    this->professors = std::vector<int>();
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

    result += "advisors: ";
    for (int i = 0; i < this->advisorInSlot.size(); i++) {
        result += std::to_string(this->advisorInSlot[i]) + " ";
    }

    result += "professors: ";
    for (int i = 0; i < this->professors.size(); i++) {
        result += std::to_string(this->professors[i]) + " ";
    }
    
    return result;
}

int Instance::getNumSlots() {
    return this->numberOfSlots;
}

int Instance::getNumProfessors() {
    return this->numberOfProfessors;
}