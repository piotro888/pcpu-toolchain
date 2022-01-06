#pragma once

#include "vga.h"
#include "sd.h"

struct cpu_state {
        unsigned short r[8] = {0,0,0,0,0,0,0,0};
        unsigned short pc = 0;
        int state_result = 0;
};

class CPU {
public:
    CPU(VGA* vga, SD* sd) : periph_vga(vga), periph_sd(sd) {}
    cpu_state state;
    void execute();

    void memWrite(unsigned short address, unsigned short data);
    unsigned short memRead(unsigned short address);
    void memWriteProgram(unsigned short address, unsigned int data);

    

    const static int RAM_SIZE = 65536;
    unsigned short ram[RAM_SIZE];
    unsigned int rom[RAM_SIZE];
private:
    VGA* periph_vga;
    SD* periph_sd;
};