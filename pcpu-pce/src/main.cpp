#include <bits/stdc++.h>

#include "vga.h"
#include "core.h"
#include "loader.h"
#include "debbuger.h"

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cerr<<"Missing arguments!";
        std::cout<<"Usage: pce [pbl kernel file] [memory map file] [iso file]\n";
        return 1;
    }

    std::ifstream file;
    
    VGA vga;
    SD sd(argv[3]);

    CPU cpu(&vga, &sd);

    Loader ld;
    file.open(argv[1]);
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


