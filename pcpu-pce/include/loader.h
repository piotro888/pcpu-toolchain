#pragma once

#include "core.h"

#include <iostream>
#include <fstream>

class Loader {
public:
    bool loadPBL(std::ifstream& file, CPU& cpu);
private:
    std::string line;
};