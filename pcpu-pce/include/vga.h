#pragma once

#include <SFML/Graphics.hpp>
#include <queue>

class VGA {
public:
    void init();
    void processEventQueue();
    void write(uint16_t addr, uint16_t data);
    void redraw();
    void submit_scancodes();
    
    bool should_close = false;
    static const int NUM_ROWS = 48, NUM_COLS = 106;
    int last_scancode;
private:
    int fast_scroll = 0;
    void keyPress(sf::Event event);
    sf::RenderWindow window;
    sf::Font font;
    sf::Text text_fg[NUM_ROWS+1][NUM_COLS]; 
    sf::RectangleShape text_bg[NUM_ROWS+1][NUM_COLS];
    std::queue <int> scancodes;
};

#define VGA_ADDR 0x1000