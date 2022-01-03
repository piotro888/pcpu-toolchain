#include "vga.h"
#include <iostream>

void VGA::init() {
    window.create(sf::VideoMode(640, 480), "PCE");
    window.display();
    //"font/SourceCodePro-Regular.ttf"
    if(!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf"))
        std::cerr<<"FONT ERR\n";

    float text_height = window.getSize().y / (float)NUM_ROWS;
    float col_step = window.getSize().x / (float)NUM_COLS;
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            text_fg[i][j].setFont(font);

            text_bg[i][j].setPosition(col_step*j, text_height*i);
            text_bg[i][j].setSize(sf::Vector2f(col_step, (float)text_height));
            text_bg[i][j].setFillColor(sf::Color::Black);

            text_fg[i][j].setCharacterSize(text_height);
            text_fg[i][j].setPosition(col_step*j, text_height*i - text_height/4.5f);
            text_fg[i][j].setFillColor(sf::Color::White);
            text_fg[i][j].setString(' ');
        }
    }
    redraw();
}

void VGA::processEventQueue() {
    sf::Event ev;
    while(window.pollEvent(ev)) {
        if(ev.type == sf::Event::Resized)
            redraw();
        if(ev.type == sf::Event::Closed) {
            window.close();
            should_close = true;
        }
    }
}

void VGA::redraw() {
    window.clear();
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {      
            window.draw(text_bg[i][j]);
            window.draw(text_fg[i][j]);
        }
    }
    window.display();
}

void VGA::write(uint16_t addr, uint16_t data) {
    int col = addr%106;
    int row = addr/106;
    
    
    if((data&0xFF) < 31) //sfml crashes
        return;

    std::string s = "";
    s += (char) (data&0xFF);
    
    text_fg[row][col].setString(s);
    data >>= 8;
    int bright = (data&0x8) ? 1 : 0;
    text_fg[row][col].setFillColor(sf::Color((data&1) ? (((data&1)<<7)<<bright)-bright : 0, (data&2) ? (((data&2)<<6)<<bright)-bright : 0, (data&4) ? (((data&4)<<5)<<bright)-bright : 0));
    data >>= 4;
    bright = (data&0x8) ? 1 : 0;
    text_bg[row][col].setFillColor(sf::Color((data&1) ? (((data&1)<<7)<<bright)-bright : 0, (data&2) ? (((data&2)<<6)<<bright)-bright : 0, (data&4) ? (((data&4)<<5)<<bright)-bright : 0));
}