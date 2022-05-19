#ifndef CORE_H
#define CORE_H

#include "vga.h"
#include "sd.h"

struct cpu_state {
        unsigned short r[8] = {0,0,0,0,0,0,0,0};
        unsigned short pc = 0;
        int state_result = 0;

        int sr1_control = 1;
        int sr2_jtr = 0, sr2_jtr_buff = 0, sr3_tmp_pc = 0, sr5_irq_flags = 0, sr6_scratch = 0;
};

const static int RAM_SIZE = (1<<20);
extern unsigned short ram[RAM_SIZE];
extern unsigned int rom[RAM_SIZE];

class CPU {
public:
    CPU(VGA* vga, SD* sd) : periph_vga(vga), periph_sd(sd) {}
    cpu_state state;
    void execute();

    void memWrite(unsigned short address, unsigned short data);
    unsigned short memRead(unsigned short address);
    void memWriteProgram(unsigned short address, unsigned int data);
    void triggerIRQ(int n);

    unsigned short page_ram[16];
    unsigned short page_rom[16];
private:
    bool pending_irq = false;
    VGA* periph_vga;
    SD* periph_sd;
    int irq_mask = 0xff, irq_trig = 0;
};

#define SR1_SUP 1
#define SR1_IMO 2
#define SR1_IRQ 4
#define SR1_MEMPAGE 8

#endif