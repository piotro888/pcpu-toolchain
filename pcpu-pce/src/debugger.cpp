#include "debbuger.h"
#include <iostream>
#include <fstream>
#include <signal.h>

using namespace std;

const string help = "Commands:\nh - help\nc - continue\ns - step\nd - step over\nb <addr> - set breakpoint\nm <addr> - dump mem\np <addr> - dump prog mem\nw - where am i?\nlb - list breakpoints\nmb - memory breakpoint\nf - continue to function call/ret";

Debugger::Debugger(CPU* _cpu) : cpu(_cpu) {
    mode = STEP; // find a way to non blocking io or signal to switch to step from run
    step_completed = true;
}

void Debugger::threadLoop() {
    for(;;)
        interactive();
}

inline unsigned int Debugger::get_real_pc() {
    return (cpu->state.sr2_jtr & 0x2) ?  
                (cpu->page_rom[(cpu->state.pc>>12)]<<12) | (cpu->state.pc & 0x0FFF) :
                 cpu->state.pc;
}

void Debugger::interactive() {
    unsigned int pc = get_real_pc();
    
    while(true) {
        while(mode == STEP && !step_completed);
        dump_state();
        pretty_command(rom[pc]);
        cout<<'\n';

        cout<<"> ";
        string inp;
        cin>>inp;

        if(inp[0] == 'h' || inp[0] == '?') {
            cout<<help<<'\n';
        } else if (inp[0] == 'c') {
            cout<<"continue execution from pc 0x"<<hex<<pc<<'\n';
            mode = RUN;
            break;
        } else if(inp[0] == 's') {
            mode = STEP;
            step_completed = false;
            break;
        } else if(inp[0] == 'd') {
            mode = STEP;
            break;
            // if current instr jump = STEP_OVER (run)
        } else if(inp[0] == 'b') {
            string addr;
            cin>>addr;
            breakpoints.insert(stoul(addr, 0, 0));
            cout<<"created code breakpoint @ 0x"<<hex<<stoul(addr, 0, 0)<<'\n';
        } else if(inp[0] == 'm') {
            string addr;
            cin>>addr;
            int iadr = stoul(addr, 0, 0);
            for(int i=0; i<8; i++) {
                cout<<hex<<"0x"<<iadr+i<<": "<<"0x"<<ram[iadr+i];
                if(ram[iadr+i] >= 0x20 && ram[iadr+i] <= 0x7F)  
                    cout<<" ("<<(char)ram[iadr+i]<<")";
                cout<<'\n';
            }
        } else if(inp[0] == 'p') {
            string addr;
            cin>>addr;
            int iadr = stoul(addr, 0, 0);
            for(int i=0; i<16; i++) {
                cout<<hex<<"0x"<<iadr+i<<": "<<"0x"<<rom[iadr+i]<<" (";
                pretty_command(rom[iadr+i]);
                cout<<")\n";
            }
        } else {
            cout<<"unknown command\n";
        }
    }
}

void Debugger::dumpOnTimer() {
    unsigned int pc = get_real_pc();

    if(state_clock.getElapsedTime().asMilliseconds() > 1000) {
        dump_state();
        pretty_command(rom[pc]);
        cout<<'\n';
        state_clock.restart();
    }
}

void Debugger::pretty_command(unsigned int instr) {
    int opcode = instr & 0x7F;
    short ia = instr>>16;
    int tg = (instr>>7)&0b111;
    int fo = (instr>>10)&0b111;
    int so = (instr>>13)&0b111;

    if(opcode == 0x0){ 
        cout<<"nop";
    } else if (opcode == 0x1){
        cout<<"mov r"<<tg<<", r"<<fo;
    } else if (opcode == 0x2){
        cout<<"ldd r"<<tg<<", 0x"<<hex<<ia;
    } else if (opcode == 0x3){
        cout<<"ldo r"<<tg<<", r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x4){
        cout<<"ldi r"<<tg<<", 0x"<<hex<<ia;
    } else if (opcode == 0x5){
        cout<<"std r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x6){
        cout<<"sto r"<<fo<<", r"<<so<<", 0x"<<hex<<ia;
    } else if (opcode == 0x7){
        cout<<"add r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x8){
        cout<<"adi r"<<tg<<", r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x9){
        cout<<"adc r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0xA){
        cout<<"sub r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0xB){
        cout<<"suc r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0xC){
        cout<<"cmp r"<<fo<<", r"<<so;
    } else if (opcode == 0xD){
        cout<<"cmi r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0xE){
        int cond_code = (instr >> 7) & 0b1111;
        if (cond_code == 0x0) cout<<"jmp";
        else if (cond_code == 0x1)  cout<<"jca";
        else if (cond_code == 0x2) cout<<"jeq";
        else if (cond_code == 0x3) cout<<"jlt";
        else if (cond_code == 0x4) cout<<"jgt";
        else if (cond_code == 0x5) cout<<"jle";
        else if (cond_code == 0x6) cout<<"jge";
        else if (cond_code == 0x7) cout<<"jne";
        cout<<" 0x"<<hex<<ia<<" (";
        pretty_prog_addr(ia);
        cout<<")";
    
    } else if (opcode == 0xF){
        cout<<"jal r"<<fo<<", 0x"<<hex<<ia<<" (";
        pretty_prog_addr(ia);
        cout<<")";
    } else if (opcode == 0x10) {
        cout<<"srl r"<<tg<<", ";
        if(ia == 0)
            cout<<"pc";
        else if(ia == 1)
            cout<<"control";
        else if(ia == 2)
            cout<<"jtr";
        else
            cout<<"unknown";
    } else if (opcode == 0x11) {
        cout<<"srs r"<<fo<<", ";
        if(ia == 0)
            cout<<"pc";
        else if(ia == 1)
            cout<<"control";
        else if(ia == 2)
            cout<<"jtr";
        else
            cout<<"unknown";
    } else if (opcode == 0x13){
        cout<<"and r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x14){
        cout<<"orr r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x15){
        cout<<"xor r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x16){
        cout<<"ani r"<<tg<<", r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x17){
        cout<<"ori r"<<tg<<", r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x18){
        cout<<"xoi r"<<tg<<", r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x19){
        cout<<"shl r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x1A){
        cout<<"shr r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x1B){
        cout<<"cai r"<<fo<<", 0x"<<hex<<ia;
    } else if (opcode == 0x1C){
        cout<<"mul r"<<tg<<", r"<<fo<<", r"<<so;
    } else if (opcode == 0x1D){
        cout<<"div r"<<tg<<", r"<<fo<<", r"<<so;
    } else {
        cout<<"illegal opcode";
    }
}

void Debugger::dump_state() {
    unsigned int pc = get_real_pc();

    cout<<"pc: 0x"<<hex<<pc<<" (";
    pretty_prog_addr(pc); cout<<") ";
    for(int i=0; i<8; i++){
        cout<<"r"<<i<<": "<<"0x"<<hex<<cpu->state.r[i]<<' ';
    }
    cout<<"\n";
}

void Debugger::pretty_prog_addr(unsigned int addr) {
    auto it = code_symbols.lower_bound(addr);
    if(it->first != addr)
        it--;
    cout<<it->second<<"+0x"<<hex<<addr-(it->first);
}

void Debugger::loadMemMap(ifstream& file) {
    string ln;
    while(file>>ln) {
        if(ln == "-p")
            break;
        
        int addr;
        file>>addr;
        if((ln[0] == 'c' && ln[1] == 'l'))
            continue; // dont store temp labels
        code_symbols[addr] = ln;
    }
}