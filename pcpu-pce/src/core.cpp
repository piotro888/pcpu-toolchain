#include "core.h"
#include <iostream>

using namespace std;

unsigned short ram[RAM_SIZE];
unsigned int rom[RAM_SIZE];

void CPU::execute() {
    unsigned int pc_addr = state.pc;
    unsigned int pc_addr_b = state.pc;
    
    if(state.sr2_jtr & 0x2)
        pc_addr = (page_rom[(state.pc>>12)]<<12) | (state.pc & 0x0FFF);
    
    unsigned int instr = rom[pc_addr];

    int opcode = instr & 0x7F;
    unsigned short ia = instr>>16;
    int tg = (instr>>7)&0b111;
    int fo = (instr>>10)&0b111;
    int so = (instr>>13)&0b111;

    state.pc++;
    if(opcode == 0x0) { 
    } else if (opcode == 0x1) {
        state.r[tg] = state.r[fo];
    } else if (opcode == 0x2) {
        state.r[tg] = memRead(ia);
    } else if (opcode == 0x3) {
        state.r[tg] = memRead(state.r[fo]+ia);
    } else if (opcode == 0x4) {
        state.r[tg] = ia;
    } else if (opcode == 0x5) {
        memWrite(ia, state.r[fo]);
    } else if (opcode == 0x6) {
        memWrite(state.r[so]+ia, state.r[fo]);
    } else if (opcode == 0x7) {
        state.state_result = (int)state.r[fo] + (int)state.r[so];
        state.r[tg] = state.r[fo] + state.r[so];
    } else if (opcode == 0x8) {
        state.state_result = (int)state.r[fo] + (int)ia;
        state.r[tg] = state.r[fo] + ia;
    } else if (opcode == 0x9) {
        if(state.state_result & (1<<17)) {
            state.state_result = (int)state.r[fo] + (int)state.r[so];
            state.r[tg] = state.r[fo] + state.r[so];
        }
    } else if (opcode == 0xA) {
        state.state_result = (int)state.r[fo] - (int)state.r[so];
        state.r[tg] = state.r[fo] - state.r[so];
    } else if (opcode == 0xB) {
        if(!(state.state_result & (1<<17))) {
            state.state_result = (int)state.r[fo] - (int)state.r[so];
            state.r[tg] = state.r[fo] - state.r[so];
        }
    } else if (opcode == 0xC) {
        state.state_result = ((int)state.r[fo]) - ((int)state.r[so]);
    } else if (opcode == 0xD) {
        state.state_result = (int)state.r[fo] - (int)ia;
    } else if (opcode == 0xE) {
        int cond_code = (instr >> 7) & 0b1111;
        if((cond_code == 0x0) ||
            (cond_code == 0x1 && (state.state_result & (1<<16))) ||
            (cond_code == 0x2 && ((short)state.state_result == 0)) ||
            (cond_code == 0x3 && ((short)state.state_result < 0 )) ||
            (cond_code == 0x4 && ((short)state.state_result > 0)) ||
            (cond_code == 0x5 && ((short)state.state_result <= 0)) ||
            (cond_code == 0x6 && ((short)state.state_result >= 0)) ||
            (cond_code == 0x7 && ((short)state.state_result != 0)) ) {
            state.pc = ia;
        }
        state.sr2_jtr = state.sr2_jtr_buff;
    } else if (opcode == 0xF) {
        state.r[tg] = state.pc - 1;
        state.pc = ia;
    } else if (opcode == 0x10) {
        if(ia == 0)
            state.r[tg] = state.pc - 1;
        else if(ia == 1)
            state.r[tg] = state.sr1_control;
        else if(ia == 2)
            state.r[tg] = state.sr2_jtr;
        else if(ia == 3)
            state.r[tg] = state.sr3_tmp_pc;
        else if(ia == 4)
            state.r[tg] = state.state_result;
        else if(ia == 5)
            state.r[tg] = state.sr5_irq_flags;
        else if(ia == 6)
            state.r[tg] = state.sr6_scratch;
    } else if (opcode == 0x11) {
        if(ia == 0) {
            state.pc = state.r[fo] + 1;
            state.sr2_jtr = state.sr2_jtr_buff;
        } else if(ia == 1 && (state.sr1_control & SR1_SUP))
            state.sr1_control = state.r[fo];
        else if(ia == 2)
            state.sr2_jtr_buff = state.r[fo];
        else if(ia == 3)
            state.sr3_tmp_pc = state.r[fo];
        else if(ia == 4)
            state.state_result = state.r[fo];
        else if(ia == 4)
            state.sr5_irq_flags = state.r[fo];
        else if(ia == 6)
            state.sr6_scratch = state.r[fo];
        else if(ia >= 0x10 && ia < 0x20)
            page_ram[ia-0x10] = state.r[fo];
        else if(ia >= 0x20 && ia < 0x30)
            page_rom[ia-0x20] = state.r[fo];
    } else if (opcode == 0x12) {
        state.pc = 0x1;
        state.sr5_irq_flags = 0b1000 | ((state.sr1_control&SR1_MEMPAGE)>0)
        | (((state.sr2_jtr & 0b10)>0)<<1) | (((state.sr1_control&SR1_SUP)>0)<<2);
        state.sr2_jtr &= ~(0b10); // disable prom page
        state.sr2_jtr_buff &= ~(0b10);
        state.sr1_control &= ~(SR1_MEMPAGE);
        state.sr1_control &= ~(SR1_IRQ);
        state.sr1_control |= (SR1_SUP);
        state.sr3_tmp_pc = pc_addr_b+1;
    } else if (opcode == 0x13) {
        state.state_result = state.r[fo] & state.r[so];
        state.r[tg] = state.r[fo] & state.r[so];
    } else if (opcode == 0x14) {
        state.state_result = state.r[fo] | state.r[so];
        state.r[tg] = state.r[fo] | state.r[so];
    } else if (opcode == 0x15) {
        state.state_result = state.r[fo] ^ state.r[so];
        state.r[tg] = state.r[fo] ^ state.r[so];
    } else if (opcode == 0x16) {
        state.state_result = state.r[fo] & ia;
        state.r[tg] = state.r[fo] & ia;
    } else if (opcode == 0x17) {
        state.state_result = state.r[fo] | ia;
        state.r[tg] = state.r[fo] | ia;
    } else if (opcode == 0x18) {
        state.state_result = state.r[fo] ^ ia;
        state.r[tg] = state.r[fo] ^ ia;
    } else if (opcode == 0x19) {
        state.r[tg] = state.r[fo]<<state.r[so];
        state.state_result = state.r[fo]<<state.r[so];
    } else if (opcode == 0x1A) {
        state.state_result = state.r[fo]>>state.r[so];
        state.r[tg] = state.r[fo]>>state.r[so];
    } else if (opcode == 0x1B) {
        state.state_result = state.r[fo] & ia;
    } else if (opcode == 0x1C) {
        state.state_result = (unsigned short)state.r[fo]*(unsigned short)state.r[so];
        state.r[tg] = (unsigned short)state.r[fo]*(unsigned short)state.r[so];
    } else if (opcode == 0x1D) {
        state.state_result =  (unsigned short)state.r[fo]/(unsigned short)state.r[so];
        state.r[tg] =  (unsigned short)state.r[fo]/(unsigned short)state.r[so];
    } else if (opcode == 0x1E) {
        state.pc = state.sr3_tmp_pc;
        state.sr1_control |= (SR1_IRQ);
        state.sr2_jtr = state.sr2_jtr_buff;
    } else if (opcode == 0x1F) {
        state.r[tg] = memRead(ia, true);
    } else if (opcode == 0x20) {
        state.r[tg] = memRead(state.r[fo]+ia, true);
    } else if (opcode == 0x21) {
        memWrite(ia, state.r[fo], true);
    } else if (opcode == 0x22) {
        memWrite(state.r[so]+ia, state.r[fo], true);
    }

    if ((pending_irq || (irq_mask & irq_trig)) && (state.sr1_control & (SR1_IRQ)) && opcode != 0x1E) {
        pending_irq = false;
        int next_pl_pc = state.pc;
        state.pc = 0x1;
                       
        state.sr5_irq_flags = 0b0000 | ((state.sr1_control&SR1_MEMPAGE)>0)
        | (((state.sr2_jtr & 0b10)>0)<<1) | (((state.sr1_control&SR1_SUP)>0)<<2);
        state.sr2_jtr &= ~(0b10); // disable prom page
        state.sr2_jtr_buff &= ~(0b10);
        state.sr1_control &= ~(SR1_MEMPAGE);
        state.sr1_control &= ~(SR1_IRQ);
        state.sr1_control |= (SR1_SUP);
        state.sr1_control |= (SR1_MEMSTD);
        state.sr3_tmp_pc = next_pl_pc;
    }
    //dbgz->dump_state();
}

void CPU::memWrite(unsigned short address_r, unsigned short data, bool mem8) {
    unsigned int address = address_r;
    if(state.sr1_control & SR1_MEMPAGE)
        address = (page_ram[(address_r>>12)]<<12) | (address_r & 0x0FFF);   
    unsigned int bus_mask = (((state.sr1_control & SR1_MEMSTD) && mem8) ? (address&1 ? 0x00ff : 0xff00) : 0xffff); // LE
    if(state.sr1_control & SR1_MEMSTD)
        address >>= 1;
    
    // todo periph
    if(!(state.sr1_control & SR1_IMO)) { 
        if(address >= 0x2682) {
            ram[address] &= (~bus_mask);
            ram[address] |= (bus_mask == 0xff00 ? data<<8 : data) & bus_mask;
        } else if (address >= 0x100 && address < 0x2682) {
            periph_vga->write(address-0x100, data);
        } else if (address == 0x4) {
            periph_sd->spi4write(data);
        } else if (address == 0x12) {
            irq_trig &= ~data;
        }
    } else { // it is ok to clarify paging access
        rom[(address>>1)] &= (address & 1) ? 0xFFFF : 0xFFFF0000;
        rom[(address>>1)] |= (address & 1) ? (data<<16) : data;
    }
}

unsigned short CPU::memRead(unsigned short address_r, bool mem8) {
    unsigned int address = address_r;
    if(state.sr1_control & SR1_MEMPAGE)
        address = (page_ram[(address_r>>12)]<<12) | (address_r & 0x0FFF);
    unsigned int bus_mask = (((state.sr1_control & SR1_MEMSTD) && mem8) ? (address&1 ? 0x00ff : 0xff00) : 0xffff);
    if(state.sr1_control & SR1_MEMSTD)
        address >>= 1;
    
    unsigned int ret = 0;
    if(address >= 0x2682) {
        ret = ram[address];
    } else if (address >= 0x0100 && address < 0x2682) {
        ret = 0;
    } else if (address == 0x3) {
        ret = periph_vga->last_scancode;
    } else if (address == 0x4) {
        ret = periph_sd->spi4read();
    } else if (address == 0x10) {
        ret = irq_trig;
    }
 
    ret &= bus_mask;
    return (bus_mask == 0xff00 ? (ret >> 8) : ret);
}

void CPU::memWriteProgram(unsigned short address, unsigned int data) {
    rom[address] = data;
}

void CPU::triggerIRQ(int n) {
    CPU::pending_irq = true;
    irq_trig |= (1<<n);
}
