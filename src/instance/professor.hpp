#ifndef PROFESSORS_HPP
#define PROFESSORS_HPP

#include <vector>

class Professor {
    private:
        int c;
        int d;
    public:
        int index;
        Professor(int index, int c, int d);
        int getMinimumSlots();
        int getMaximumSlots();

        void setMinimumSlots(int c);
        void setMaximumSlots(int d);

        void incrementMinimumSlots();
        void incrementMaximumSlots();
};

#endif