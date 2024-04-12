#include "professor.hpp"

Professor::Professor(int index, int c, int d): index(index), c(c), d(d) {}

int Professor::getMinimumSlots() {
    return this->c;
}

int Professor::getMaximumSlots() {
    return this->d;
}

void Professor::setMinimumSlots(int c) {
    this->c = c;
}

void Professor::setMaximumSlots(int d) {
    this->d = d;
}

void Professor::incrementMinimumSlots() {
    this->c++;
}

void Professor::incrementMaximumSlots() {
    this->d++;
}
