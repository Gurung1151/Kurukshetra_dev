//
// Created by samip on 27/2/19.
//

#pragma once


#include <SFML/Graphics/Text.hpp>
#include "MainMenu.h"

class IpEnter {
public:
    IpEnter(unsigned int width, unsigned int height);
    void render();
    void run();
    void handlePlayerInput(sf::Keyboard::Key &key, bool isPressed);
    void processEvents();
    void onPressEnter();
private:
    sf::Text menu[3];
    ResourceHolder <sf::Font, Fonts::ID> fonts;
    sf::RenderWindow mWindow2;
    sf::RectangleShape inputBox;
    sf::String playerInput;
    sf::Text playerText,enter,IpShow,name;
    sf::Texture ipTexture;
    sf::RectangleShape ipImage;
    sf::String nameInput;
    bool isPressedReturn;
    unsigned width, height;
};

