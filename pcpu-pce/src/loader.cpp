#include "loader.h"

#include <string.h>


bool Loader::loadPBL(std::ifstream& file, CPU& cpu) {
    file>>line; // one line

    if(line[0] != '*')
        std::cerr<<"invalid file";

    // rom entries
    int rom_addr = 0, spos = 0;
    for(unsigned int i=1; i<line.size(); i+=8) {
        if(line[i] == '*') {
            spos = i+1;
            break;
        }
        
        unsigned int entry = std::stoul(line.substr(i, 8), nullptr, 16);
        cpu.memWriteProgram(rom_addr, entry);
        rom_addr++;
    }
    
    int ram_loc = 0x5000/2;
    for(unsigned int i=spos; i<line.size(); i+=4) {
        if(line[i] == '*') {
            break;
        }

        int num = std::stoi(line.substr(i, 4), nullptr, 16);
        cpu.memWrite(ram_loc, num);
        ram_loc++;
    }
    return true;
}
