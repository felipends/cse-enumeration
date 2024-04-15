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
    std::cout << "Slot: " << slot << "tamanho advisorInSlot: " << this->advisorInSlot.size() << "\n";
    std::cout << "Indice professor: " << professor - 1 << "tamanhoProfessor: " << this->professors.size() << "\n";
    this->professors[professor - 1]->incrementMinimumSlots();
}

std::string Instance::toString() {
    std::string result = "";

    result += "advisors: ";
    for (int i = 0; i < this->advisorInSlot.size(); i++) {
        result += std::to_string(this->advisorInSlot[i]) + " ";
    }

    result += "professors: ";
    for (int i = 0; i < this->professors.size(); i++) {
        result += "index: " + std::to_string(this->professors[i]->index) + " ";
    }
    
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
