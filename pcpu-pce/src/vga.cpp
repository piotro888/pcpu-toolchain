#include "vga.h"
#include <iostream>
#include "core.h"

void VGA::init() {
    //window.create(sf::VideoMode(640, 480), "PCE", sf::Style::Fullscreen);
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
        if(ev.type == sf::Event::KeyPressed || ev.type == sf::Event::KeyReleased) {
            keyPress(ev);
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
    if(addr == 0x3001) {
        int diff = data - fast_scroll;
        fast_scroll = data;
        if(diff < 0)
            diff += NUM_ROWS;

        for(int z=0; z<diff; z++) {
            for(int j=0; j<NUM_COLS; j++) {
                text_fg[NUM_ROWS][j].setString(text_fg[0][j].getString());
                text_fg[NUM_ROWS][j].setFillColor(text_fg[0][j].getFillColor());
                text_bg[NUM_ROWS][j].setFillColor(text_bg[0][j].getFillColor());
            }

            for(int i=0; i<NUM_ROWS; i++) {
                for(int j=0; j<NUM_COLS; j++) {
                    text_fg[i][j].setString(text_fg[i+1][j].getString());
                    text_fg[i][j].setFillColor(text_fg[i+1][j].getFillColor());
                    text_bg[i][j].setFillColor(text_bg[i+1][j].getFillColor());
                }
            }
        }

        return;
    }

    int col = addr%106;
    int row = addr/106;
    row += NUM_ROWS-fast_scroll;
    row %= NUM_ROWS;

    if(row >= 48)
        return;

    std::string s = "";
    if((data&0xFF) > 31 && (data&0xFF) != 0xFF) // sfml crashes
        s += (char) (data&0xFF);
    
    text_fg[row][col].setString(s);
    data >>= 8;
    int bright = (data&0x8) ? 1 : 0;
    text_fg[row][col].setFillColor(sf::Color((data&1) ? (((data&1)<<7)<<bright)-bright : 0, (data&2) ? (((data&2)<<6)<<bright)-bright : 0, (data&4) ? (((data&4)<<5)<<bright)-bright : 0));
    data >>= 4;
    bright = (data&0x8) ? 1 : 0;
    text_bg[row][col].setFillColor(sf::Color((data&1) ? (((data&1)<<7)<<bright)-bright : 0, (data&2) ? (((data&2)<<6)<<bright)-bright : 0, (data&4) ? (((data&4)<<5)<<bright)-bright : 0));
}

void VGA::keyPress(sf::Event event) {
    sf::Keyboard::Key k = event.key.code;

    // extended
    if(k == sf::Keyboard::Left || k == sf::Keyboard::Right || k == sf::Keyboard::Up 
            || k == sf::Keyboard::Down || k == sf::Keyboard::RControl) {
        scancodes.push(0xe0);
    }

    if(event.type == sf::Event::KeyReleased) {
        scancodes.push(0xf0);
    }
    
    // translate to PS/2 scancodes
    if     (k == sf::Keyboard::Q)   scancodes.push(0x15);
    else if(k == sf::Keyboard::W)   scancodes.push(0x1d);
    else if(k == sf::Keyboard::E)   scancodes.push(0x24);
    else if(k == sf::Keyboard::R)   scancodes.push(0x2d);
    else if(k == sf::Keyboard::T)   scancodes.push(0x2c);
    else if(k == sf::Keyboard::Y)   scancodes.push(0x35);
    else if(k == sf::Keyboard::U)   scancodes.push(0x3c);
    else if(k == sf::Keyboard::I)   scancodes.push(0x43);
    else if(k == sf::Keyboard::O)   scancodes.push(0x44);
    else if(k == sf::Keyboard::P)   scancodes.push(0x4d);
    else if(k == sf::Keyboard::A)   scancodes.push(0x1c);
    else if(k == sf::Keyboard::S)   scancodes.push(0x1b);
    else if(k == sf::Keyboard::D)   scancodes.push(0x23);
    else if(k == sf::Keyboard::F)   scancodes.push(0x2b);
    else if(k == sf::Keyboard::G)   scancodes.push(0x34);
    else if(k == sf::Keyboard::H)   scancodes.push(0x33);
    else if(k == sf::Keyboard::J)   scancodes.push(0x3b);
    else if(k == sf::Keyboard::K)   scancodes.push(0x42);
    else if(k == sf::Keyboard::L)   scancodes.push(0x4b);
    else if(k == sf::Keyboard::Z)   scancodes.push(0x1a);
    else if(k == sf::Keyboard::X)   scancodes.push(0x22);
    else if(k == sf::Keyboard::C)   scancodes.push(0x21);
    else if(k == sf::Keyboard::V)   scancodes.push(0x2a);
    else if(k == sf::Keyboard::B)   scancodes.push(0x32);
    else if(k == sf::Keyboard::N)   scancodes.push(0x31);
    else if(k == sf::Keyboard::M)   scancodes.push(0x3a);
    else if(k == sf::Keyboard::Num0)   scancodes.push(0x45);
    else if(k == sf::Keyboard::Num1)   scancodes.push(0x16);
    else if(k == sf::Keyboard::Num2)   scancodes.push(0x1e);
    else if(k == sf::Keyboard::Num3)   scancodes.push(0x26);
    else if(k == sf::Keyboard::Num4)   scancodes.push(0x25);
    else if(k == sf::Keyboard::Num5)   scancodes.push(0x2e);
    else if(k == sf::Keyboard::Num6)   scancodes.push(0x36);
    else if(k == sf::Keyboard::Num7)   scancodes.push(0x3d);
    else if(k == sf::Keyboard::Num8)   scancodes.push(0x3e);
    else if(k == sf::Keyboard::Num9)   scancodes.push(0x46);
    else if(k == sf::Keyboard::Tilde)  scancodes.push(0x0e);
    else if(k == sf::Keyboard::Subtract)   scancodes.push(0x4e);
    else if(k == sf::Keyboard::Equal)   scancodes.push(0x55);
    else if(k == sf::Keyboard::BackSpace)   scancodes.push(0x66);
    else if(k == sf::Keyboard::Tab)   scancodes.push(0x0d);
    else if(k == sf::Keyboard::LBracket)   scancodes.push(0x54);
    else if(k == sf::Keyboard::RBracket)   scancodes.push(0x5b);
    else if(k == sf::Keyboard::BackSlash)  scancodes.push(0x5d);
    else if(k == sf::Keyboard::SemiColon)  scancodes.push(0x4c);
    else if(k == sf::Keyboard::Quote)  scancodes.push(0x52);
    else if(k == sf::Keyboard::Comma)  scancodes.push(0x49);
    else if(k == sf::Keyboard::Period)  scancodes.push(0x41);
    else if(k == sf::Keyboard::Slash)  scancodes.push(0x4a);
    else if(k == sf::Keyboard::LControl) scancodes.push(0x14);
    else if(k == sf::Keyboard::LShift)  scancodes.push(0x12);
    else if(k == sf::Keyboard::RShift) scancodes.push(0x59);
    else if(k == sf::Keyboard::Enter)  scancodes.push(0x5a);
    else if(k == sf::Keyboard::Space)  scancodes.push(0x29);
    else if(k == sf::Keyboard::Left)  {scancodes.push(0x6b);}
    else if(k == sf::Keyboard::Right)  {scancodes.push(0x74);}
    else if(k == sf::Keyboard::Up)  {scancodes.push(0x75);}
    else if(k == sf::Keyboard::Down)  {scancodes.push(0x72);}
    else if(k == sf::Keyboard::RControl)  {scancodes.push(0x14);}
}

extern CPU* t_cpu;
void VGA::submit_scancodes() {
    if(scancodes.empty())
        return;
    
    last_scancode = scancodes.front();
    scancodes.pop();
    t_cpu->triggerIRQ(0);
}