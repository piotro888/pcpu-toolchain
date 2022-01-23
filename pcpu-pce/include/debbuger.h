#pragma once

#include "core.h"
#include <atomic>
#include <unordered_set>

class Debugger {
public:
    Debugger(CPU* _cpu);
    
    enum mode { RUN, STEP, STEP_OVER } mode;
    std::atomic <bool> step_completed;

    void interactive();

    void loadMemMap(std::ifstream& file);
    void threadLoop();
    void dumpOnTimer();
private:
    void pretty_command(unsigned int cmd);
    void dump_state();
    void pretty_prog_addr(unsigned int addr);
    unsigned int get_real_pc();
    
    sf::Clock state_clock;
    std::unordered_set <int> breakpoints;
    std::map <unsigned int, std::string> code_symbols;
    std::map <unsigned int, std::string> mem_symbols;
    CPU* cpu;
};