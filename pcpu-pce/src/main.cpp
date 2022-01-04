#include <bits/stdc++.h>

#include "vga.h"
#include "core.h"
#include "loader.h"
#include "debbuger.h"

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr<<"Missing arguments!";
        std::cout<<"Usage: pce [pbl kernel file] [memory map file]";
        return 1;
    }

    std::ifstream file;
    file.open(argv[1]);
    
    VGA vga;

    CPU cpu(&vga);

    Loader ld;
    ld.loadPBL(file, cpu);
    file.close();

    Debugger dbg(&cpu);
    file.open(argv[2]);
    dbg.loadMemMap(file);
    file.close();

    vga.init();
    
    sf::Clock render_clock;
    // execution loop
    while(1) {
        cpu.execute();

        if(render_clock.getElapsedTime().asMilliseconds() >= 50) { // render at 20 fps
            vga.processEventQueue();
            vga.redraw();

            if(vga.should_close)
                break;

            render_clock.restart();
        }

        dbg.interactive();
    }
    return 0;
}


