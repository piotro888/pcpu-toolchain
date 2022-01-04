#include "sd.h"

#include <iostream>
#include <fstream>

SD::SD(char* iso) {
    isf.open(iso);
}

void SD::spi4write(uint16_t data) {
    for(int i=0; i<5; i++)
        cmd[i] = cmd[i+1];
    cmd[5] = data;

    if(data == 0xff)
        last_ff = 0;
    else
        last_ff++;

    if(q.empty())  {
        rxbit = 0xff;
    } else {
        rxbit = q.front();
        q.pop();
    }

    if(last_ff > 5) { // or 6

        //std::cout<<std::hex<<cmd[0]<<' '<<cmd[1]<<' '<<cmd[2]<<' '<<cmd[3]<<' '<<cmd[4]<<'\n';
        while(!q.empty())
            q.pop();
        
        if(cmd[0] == 0x51) { // CMD17 - read
            int loc = (cmd[1]<<24) | (cmd[2]<<16) | (cmd[3]<<8) | (cmd[4]);
            q.push(0);
            q.push(0xff);
            q.push(0xfe);
            isf.seekg(loc*512);
               
            for(int i=0; i<512; i++) {
                char c;
                isf.read(&c, 1);
                q.push(c);
            }
            q.push(0);
            q.push(0);
        } else if(cmd[0] == 0x40 || cmd[0] == 0x40 + 55) {
            q.push(0x1);
        } else if(cmd[0] == 0x48) {
            q.push(0x1);
            q.push(0x0);
            q.push(0x0);
            q.push(0x1);
            q.push(cmd[4]);
        } else if(cmd[0] == 0x40 + 41) {
            q.push(0x0);
        } else if(cmd[0] == 0x40 + 58) {
            q.push(0x0);
            q.push(0x80);
            q.push(0x10);
            q.push(0x0);
            q.push(0x0);
        } else if(cmd[0] == 0x50 && cmd[3] == 0x2 && cmd[4] == 0x0) {
            q.push(0x0);
        }
    }
}

uint16_t SD::spi4read() {
    return 0x8000 | rxbit;
}
