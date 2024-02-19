#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <string>

class Instance {
    public:
        Instance(int numberOfSlots, int numberOfProfessors);
        std::vector<int> professors; 
        std::vector<int> advisorInSlot;

        void delegate(int professor, int slot);

        std::string toString();
};

#endif
