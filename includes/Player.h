//
// Created by safal on 13/2/19.
//

#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "Animation.h"
#include "Collider.h"
#include <queue>
class Player {
public:
    void SetData(sf::Texture* playerTexture, sf::Texture* bulletTexture, sf::Vector2u imageCount, float switchTime, float speed,
                 float baseHeight);
    void Update(float deltaTime, sf::View& gameView, float &baseHeight, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    Collider GetCollider() { return Collider(body); }
    void SetPosition(sf::Vector2f position);
    void setName(const std::string& name);
    void setTimeout(sf::Time time);
    void setConnected(bool status);
    void setPing(unsigned short ping);
    sf::Vector2f getPosition();
    sf::TcpSocket* getSocket();
    sf::Time getTimeout();
    std::string getName();
    bool isConnected();
    unsigned short getPing();
    int getId();

protected:
    sf::RectangleShape body, bullet;
    Animation animation;
    unsigned row;
    bool faceRight, isJumping, isShooting;
    sf::Vector2f velocity;
    sf::Vector2f m_position;
    sf::Time m_timeout;
    std::string m_name;
    std::unique_ptr<sf::TcpSocket> m_socket = nullptr;
    bool m_connected;
    int m_id;
    unsigned short m_ping;
public:
	unsigned health, mana;
};
