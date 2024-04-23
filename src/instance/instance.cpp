#include "instance.hpp"
#include <iostream>

Instance::Instance(int numberOfSlots, int numberOfProfessors): numberOfSlots(numberOfSlots), numberOfProfessors(numberOfProfessors) {
    this->professors = std::vector<Professor*>();
    for (int i = 1; i <= numberOfProfessors; i++) {
        auto professor = new Professor(i, 0, 0);
        this->professors.push_back(professor);
    }

    this->advisorInSlot = std::vector<int>(numberOfSlots);
}

void Instance::delegate(int professor, int slot) {
    this->advisorInSlot[slot] = professor;
    this->professors[professor - 1]->incrementMinimumSlots();
    if (this->professors[professor- 1]->getMinimumSlots() > this->professors[professor- 1]->getMaximumSlots()) {
        this->professors[professor- 1]->setMaximumSlots(this->professors[professor- 1]->getMinimumSlots());
    }
}

std::string Instance::toString() {
    std::string result = "";

    result += "advisors: ";
    for (int i = 0; i < this->advisorInSlot.size(); i++) {
        result += std::to_string(this->advisorInSlot[i]) + " ";
    }

    result += "{ professors: ";
    for (int i = 0; i < this->professors.size(); i++) {
        result += "{ index: " + std::to_string(this->professors[i]->index) + " ";
        result += "c: " + std::to_string(this->professors[i]->getMinimumSlots()) + " ";
        result += "d: " + std::to_string(this->professors[i]->getMaximumSlots()) + "} ";
    }
    result += " }";
    
    return result;
}

int Instance::getNumSlots() {
    return this->numberOfSlots;
}

int Instance::getNumProfessors() {
    return this->numberOfProfessors;
}

Professor* Instance::getProfessor(int index) {
    return this->professors[index];
}