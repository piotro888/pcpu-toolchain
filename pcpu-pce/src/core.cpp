#include <core.h>
#include <iostream>

using namespace std;

void CPU::execute() {
    unsigned int instr = rom[state.pc];
    int opcode = instr & 0x7F;
    short ia = instr>>16;
    int tg = (instr>>7)&0b111;
    int fo = (instr>>10)&0b111;
    int so = (instr>>13)&0b111;

    state.pc++;
    if(opcode == 0x0) { 
    } else if (opcode == 0x1) {
        state.r[tg] = state.r[fo];
    } else if (opcode == 0x2) {
        state.r[tg] = ram[ia];
    } else if (opcode == 0x3) {
        state.r[tg] = ram[state.r[fo]+ia];
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
        if((cond_code == 0x0) |
            (cond_code == 0x1 && (state.state_result & (1<<16))) ||
            (cond_code == 0x2 && ((short)state.state_result == 0)) ||
            (cond_code == 0x3 && ((short)state.state_result < 0 )) ||
            (cond_code == 0x4 && ((short)state.state_result > 0)) ||
            (cond_code == 0x5 && ((short)state.state_result <= 0)) ||
            (cond_code == 0x6 && ((short)state.state_result >= 0)) ||
            (cond_code == 0x7 && (state.state_result != 0)) ) {
            state.pc = ia;
        }
    } else if (opcode == 0xF) {
        state.r[tg] = state.pc;
        state.pc = ia;

    } else if (opcode == 0x10) {
        if(ia == 0)
            state.r[tg] = state.pc+1;
    } else if (opcode == 0x11) {
        if(ia == 0) 
            state.pc = state.r[fo];
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
    } else {
    }
}

void CPU::memWrite(unsigned short address, unsigned short data) {
    if(address >= 0x4c00) {
        ram[address] = data;
    } else if (address >= 0x1000 && address < 0x4c00) {
        periph_vga->write(address-0x1000, data);
    }
}

void CPU::memWriteProgram(unsigned short address, unsigned int data) {
    rom[address] = data;
}