#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <string>
#include "professor.hpp"

class Instance {
    private:
        int numberOfSlots;
        int numberOfProfessors;
    public:
        Instance(int numberOfSlots, int numberOfProfessors);
        std::vector<Professor*> professors; 
        std::vector<int> advisorInSlot;

        void delegate(int professor, int slot);

        int getNumSlots();
        int getNumProfessors();
        std::string toString();

        Professor* getProfessor(int index);
};

#endif
