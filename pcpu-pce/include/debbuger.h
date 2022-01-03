#pragma once

#include <unordered_set>
#include "core.h"

class Debugger {
public:
    Debugger(CPU* _cpu);
    
    enum mode { RUN, STEP, STEP_OVER } mode;

    void interactive();
private:
    void pretty_command(unsigned int cmd);
    void dump_state();

    sf::Clock state_clock;
    std::unordered_set <int> breakpoints;
    CPU* cpu;
};