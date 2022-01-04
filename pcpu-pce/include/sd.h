#pragma once

#include <fstream>
#include <queue>

class SD {
public:
    SD(char* path);
    void spi4write(uint16_t data);
    uint16_t spi4read();
private:
    int cmd[6] = {0, 0, 0, 0, 0, 0};
    int last_ff = 0;
    int rxbit = 0xff;

    std::ifstream isf;
    std::queue<int> q;
};