#include <bits/stdc++.h>

#include "vga.h"
#include "core.h"
#include "loader.h"
#include "debbuger.h"

CPU* t_cpu;

int main(int argc, char* argv[]) {
    if(argc < 4) {
        std::cerr<<"Missing arguments!";
        std::cout<<"Usage: pce [pbl kernel file] [memory map file] [iso file]\n";
        return 1;
    }
    bool start_run = false;
    if(argc > 4 && !strcmp(argv[4], "--run"))
        start_run = true;

    std::ifstream file;
    
    VGA vga;
    SD sd(argv[3]);

    CPU cpu(&vga, &sd);
    t_cpu = &cpu;

    Loader ld;
    file.open(argv[1]);
    ld.loadPBL(file, cpu);
    file.close();

    Debugger dbg(&cpu, start_run);
    file.open(argv[2]);
    dbg.loadMemMap(file);
    file.close();

    vga.init();
    
    std::thread debugger_thread(&Debugger::threadLoop, &dbg);

    sf::Clock render_clock;
    sf::Clock irq_clock;
    // execution loop
    while(1) {
        if(dbg.mode == Debugger::mode::RUN) {
            cpu.execute();
        } else if(dbg.mode == Debugger::mode::STEP && !dbg.step_completed) {
            cpu.execute();
            dbg.step_completed = true;
        }

        if(dbg.breakpoints.count(cpu.state.pc) && dbg.mode != Debugger::mode::STEP) {
            dbg.mode = Debugger::mode::STEP;
            std::cout<<"paused at breakpoint 0x"<<std::hex<<cpu.state.pc<<"\n";
        }

        if(render_clock.getElapsedTime().asMilliseconds() >= 20) {
            vga.processEventQueue();
            vga.redraw();
            vga.submit_scancodes();

            if(vga.should_close)
                break;

            if(dbg.mode == Debugger::mode::RUN)
                dbg.dumpOnTimer();
            render_clock.restart();
        }

        if(irq_clock.getElapsedTime().asMilliseconds() > 100 && dbg.mode == Debugger::RUN) {
            // trigger timer irq
            cpu.triggerIRQ(1);
            irq_clock.restart();
        }
    }
    return 0;
}
