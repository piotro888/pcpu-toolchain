#pragma once

#include <unordered_set>
#include "core.h"

class Debugger {
public:
    Debugger(CPU* _cpu);
    
    enum mode { RUN, STEP, STEP_OVER } mode;

    void interactive();

    void loadMemMap(std::ifstream& file);
private:
    void pretty_command(unsigned int cmd);
    void dump_state();
    void pretty_prog_addr(unsigned int addr);

    sf::Clock state_clock;
    std::unordered_set <int> breakpoints;
    std::map <unsigned int, std::string> code_symbols;
    std::map <unsigned int, std::string> mem_symbols;
    CPU* cpu;
};