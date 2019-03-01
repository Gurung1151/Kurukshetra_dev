//
// Created by samip on 27/2/19.
//

#include "IpEnter.h"
void IpEnter::render(){
    mWindow2.clear();
//    mWindow2.draw(ipImage);
    mWindow2.draw(inputBox);
    mWindow2.draw(IpShow);
    mWindow2.draw(enter);
    mWindow2.draw(playerText);
    mWindow2.draw(name);
    mWindow2.display();
}
IpEnter::IpEnter(unsigned int width, unsigned int height)
        :fonts(Fonts::fontNumber),
         isPressedReturn(false),
         mWindow2(sf::VideoMode(width, height), "Kurukshetra"),
         width(width),height(height)
{
    fonts.load(Fonts::menuFont, "../Media/Fonts/DejaVuSans.ttf");
    name.setFillColor(sf::Color::Red);
    name.setPosition(300,0);
    name.setString("Hello");
    IpShow.setString("Your IP is :" +sf::IpAddress::getLocalAddress().toString());
    IpShow.setPosition(0,0);
    IpShow.setFont(fonts.get(Fonts::menuFont));
    enter.setString("Please Enter other's IP:");
    enter.setPosition(0,IpShow.getCharacterSize());
    enter.setFont(fonts.get(Fonts::menuFont));
    playerText.setPosition(0,enter.getCharacterSize());
    playerText.setFont(fonts.get(Fonts::menuFont));
    inputBox.setSize(sf::Vector2f(500, 50));
    inputBox.setOutlineThickness(2.f);
    inputBox.setFillColor(sf::Color(167, 150, 150));
    inputBox.setOrigin((mWindow2.getSize().x / 2.f) + (inputBox.getSize().x / 2.f), mWindow2.getSize().y / 2.f);
    inputBox.setPosition(mWindow2.getSize().x, mWindow2.getSize().y);
    enter.setPosition(inputBox.getOrigin().x - (inputBox.getSize().x * (4.f / 5.f)), inputBox.getOrigin().y - 100);
    playerText.setPosition(inputBox.getOrigin().x - (inputBox.getSize().x * (4.f / 5.f)),inputBox.getOrigin().y);
    playerText.setFillColor(sf::Color::Black);
    ipTexture.loadFromFile("../Media/Textures/menu.png");
    ipImage.setSize(sf::Vector2f(width, height));
    ipImage.setPosition(0,0);
    ipImage.setTexture(&ipTexture);
}
void IpEnter::handlePlayerInput(sf::Keyboard::Key &key, bool isPressed) {
        switch (key){
            case sf::Keyboard::Return:
                onPressEnter();
                break;
            case sf::Keyboard::Escape:
                mWindow2.close();
                MainMenu mainMenu(1377, 720);
                mainMenu.run();
                break;
        }
    }
void IpEnter::onPressEnter()
{
    if(!playerInput.isEmpty())
    {
        mWindow2.close();
        Game game(width, height,playerInput,nameInput);
        game.run();
    }
    else
        enter.setString("You must Enter the IP:");
}
void IpEnter::run() {
    while (mWindow2.isOpen())
    {
        processEvents();
        render();
    }
}
void IpEnter::processEvents() {
    sf::Event event = {};
    while (mWindow2.pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::TextEntered:
                if((event.text.unicode>=48 && event.text.unicode<=57)||(event.text.unicode==46))
                {
                    playerInput+=event.text.unicode;
                    playerText.setString(playerInput);
                }else if(event.text.unicode==8 && !playerInput.isEmpty())
                {
                    playerInput.erase(playerInput.getSize()-1,1);
                    playerText.setString(playerInput);
                }
                if((event.text.unicode>=65 && event.text.unicode<=90)||(event.text.unicode>=97 && event.text.unicode<=122))
                {
                    nameInput+=event.text.unicode;
                    name.setString(nameInput);
                }
                else if(event.text.unicode==8 && !nameInput.isEmpty())
                {
                    nameInput.erase(nameInput.getSize()-1,1);
                    name.setString(nameInput);
                }
//            mWindow2.draw(playerText);
                break;
            case sf::Event::Closed:
                mWindow2.close();
                break;
        }
    }
}