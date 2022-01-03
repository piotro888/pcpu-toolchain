#pragma once

#include <SFML/Graphics.hpp>

class VGA {
public:
    void init();
    void processEventQueue();
    void write(uint16_t addr, uint16_t data);
    void redraw();
    
    bool should_close = false;
    static const int NUM_ROWS = 48, NUM_COLS = 106;
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text text_fg[NUM_ROWS][NUM_COLS]; 
    sf::RectangleShape text_bg[NUM_ROWS][NUM_COLS];
};

#define VGA_ADDR 0x1000