//
// Created by safal on 13/2/19.
//

#include <iostream>
#include <cmath>
#include <GameServer.h>
#include <Player.h>

#include "Player.h"



void Player::SetData(sf::Texture *playerTexture, sf::Vector2u imageCount, float switchTime, float speed, sf::Vector2f position) {
	animation.SetData(playerTexture, imageCount, switchTime);
	row = 0;
	faceRight = true;
	isJumping = isShooting = false;
	health = mana = 100;
	bulletVelocity = moveDirection = sf::Vector2f(0.f, 0.f);

	const sf::Vector2f playerSize = sf::Vector2f(100.0f, 150.0f);
	body.setSize(playerSize);
	body.setPosition(position);
	body.setTexture(playerTexture);
	body.setOrigin(playerSize);
//    body.setOrigin(playerSize.x/2, playerSize.y/2);

//	const sf::Vector2f bulletSize = sf::Vector2f(20.f, 12.4f);
//	bullet.setSize(bulletSize);
//	bullet.setPosition(position.x+50, position.y-50);
//	bullet.setTexture(bulletTexture);
////    bullet.setOrigin(bulletSize);
//	bullet.setOrigin(bulletSize.x/2, bulletSize.y/2);

	velocity = sf::Vector2f(2*speed, 1.5f*speed);
}
void Player::Update(sf::Texture* bulletTexture, float deltaTime, sf::View &gameView, float &baseHeight, sf::RenderWindow& window)
{
	static sf::Vector2f movement(0.f, 0.f);
	static float localVelocity = velocity.y;
	const float g = 9.81f;
	if (not isJumping and sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= velocity.x * deltaTime;
		//server.update(movement.x);
	}
	if (not isJumping and sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += velocity.x * deltaTime;
		//server.update(movement.x);
	}
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and isUp)
//        movement.y += velocity.x * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		isJumping = true;
		//server.update(movement.x);
	}

//	static sf::Vector2f mousePos;
	if (not isShooting and sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i pixelMousePos = sf::Mouse::getPosition(window);

		sf::IntRect viewport = window.getViewport(gameView);
		if (viewport.contains(pixelMousePos))
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(pixelMousePos);
			std::cout<<mousePos.y<<" "<<body.getPosition().y<<std::endl;
			if (mousePos.y<=baseHeight)
			{
				sf::Vector2f localBulletPos = body.getPosition();

				sf::Vector2f displacement = sf::Vector2f(mousePos.x - localBulletPos.x, mousePos.y - localBulletPos.y);
				double distance = sqrt(double(displacement.x * displacement.x + displacement.y * displacement.y));

				moveDirection = sf::Vector2f(float(displacement.x / distance), float(displacement.y / distance));

				bulletVelocity = sf::Vector2f(4*velocity.x * moveDirection.x*deltaTime, 4*velocity.y * moveDirection.y* deltaTime);
				isShooting = true;
				clock.restart();
			}
		}
	}

//		if (isShooting and not isUp(bullet, baseHeight))
////	{
////		sf::Vector2f bulletPos = bullet.getPosition();
////
////		bool checkX, checkY;
////		if (moveDirection.x<0.f)
////			checkX = bulletPos.x<=mousePos.x;
////		else
////			checkX = bulletPos.x >= mousePos.x;
////
////		if (moveDirection.y<0.f)
////			checkY = bulletPos.y<=mousePos.y;
////		else
////			checkY = bulletPos.y >= mousePos.y;
//
////		if(checkX and checkY and not isUp(bullet, baseHeight))
//		{
//			isShooting = false;
//			bullet.setPosition(1000.f, 1.4f * (600.f - 50.f));
//			bulletVelocity.y = 0;
//		}
//		else if (isShooting)
//		{
//			bulletMovement.x += bulletVelocity.x * deltaTime;
//			bulletMovement.y += bulletVelocity.y * deltaTime;
//			bulletVelocity.y += g;
//		}
//	}
	if (isShooting and clock.getElapsedTime().asSeconds()>=0.2f )
	{
		bullets.emplace_back(Bullet(bulletTexture, sf::Vector2f(20.f, 12.4f), body.getPosition(), bulletVelocity));
		isShooting = false;
		clock.restart();
	}
	if (not isUp(body, baseHeight))
	{
		body.setPosition(body.getPosition().x, baseHeight);
		isJumping = false;
		localVelocity = velocity.y;
	}

	if(movement.x == 0)
		row = 0;
	else
	{
		row = 1;
		faceRight = movement.x > 0;
	}
	gameView.move(movement);
	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(movement);

	if (isJumping)
	{
		movement.y = -localVelocity * deltaTime;
		localVelocity -= g;
	}
	else
		movement = sf::Vector2f(0.f,0.f);
}

void Player::Update(sf::Texture* bulletTexture, float deltaTime, sf::View &gameView, float &baseHeight, sf::RenderWindow& window, GameServer& server)
{
	static sf::Vector2f movement(0.f, 0.f);
	static float localVelocity = velocity.y;
	const float g = 9.81f;
	if (not isJumping and sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= velocity.x * deltaTime;
		server.update(movement.x);
	}
	if (not isJumping and sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += velocity.x * deltaTime;
		server.update(movement.x);
	}
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and isUp)
//        movement.y += velocity.x * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		isJumping = true;
		server.update(movement.x);

	}

//	static sf::Vector2f mousePos;
	if (not isShooting and sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i pixelMousePos = sf::Mouse::getPosition(window);

		sf::IntRect viewport = window.getViewport(gameView);
		if (viewport.contains(pixelMousePos))
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(pixelMousePos);
			std::cout<<mousePos.y<<" "<<body.getPosition().y<<std::endl;
			if (mousePos.y<=baseHeight)
			{
				sf::Vector2f localBulletPos = body.getPosition();

				sf::Vector2f displacement = sf::Vector2f(mousePos.x - localBulletPos.x, mousePos.y - localBulletPos.y);
				double distance = sqrt(double(displacement.x * displacement.x + displacement.y * displacement.y));

				moveDirection = sf::Vector2f(float(displacement.x / distance), float(displacement.y / distance));

				bulletVelocity = sf::Vector2f(4*velocity.x * moveDirection.x*deltaTime, 4*velocity.y * moveDirection.y* deltaTime);
				isShooting = true;

			}
		}
	}

//		if (isShooting and not isUp(bullet, baseHeight))
////	{
////		sf::Vector2f bulletPos = bullet.getPosition();
////
////		bool checkX, checkY;
////		if (moveDirection.x<0.f)
////			checkX = bulletPos.x<=mousePos.x;
////		else
////			checkX = bulletPos.x >= mousePos.x;
////
////		if (moveDirection.y<0.f)
////			checkY = bulletPos.y<=mousePos.y;
////		else
////			checkY = bulletPos.y >= mousePos.y;
//
////		if(checkX and checkY and not isUp(bullet, baseHeight))
//		{
//			isShooting = false;
//			bullet.setPosition(1000.f, 1.4f * (600.f - 50.f));
//			bulletVelocity.y = 0;
//		}
//		else if (isShooting)
//		{
//			bulletMovement.x += bulletVelocity.x * deltaTime;
//			bulletMovement.y += bulletVelocity.y * deltaTime;
//			bulletVelocity.y += g;
//		}
//	}
	if (isShooting)
	{
		bullets.emplace_back(bulletTexture, sf::Vector2f(20.f, 12.4f), body.getPosition(), bulletVelocity);
		isShooting = false;
	}
	if (not isUp(body, baseHeight))
	{
		body.setPosition(body.getPosition().x, baseHeight);
		isJumping = false;
		localVelocity = velocity.y;
	}

	if(movement.x == 0)
		row = 0;
	else
	{
		row = 1;
		faceRight = movement.x > 0;
	}
	gameView.move(movement);
	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(movement);

	if (isJumping)
	{
		movement.y = -localVelocity * deltaTime;
		localVelocity -= g;
	}
	else
		movement = sf::Vector2f(0.f,0.f);
}

void Player::Draw(sf::RenderWindow &window) {
//	if (isDead())
//	{
//		std::cout<<"Player died successfully!"<<std::endl;
//		exit(0);
//	}
	window.draw(body);
	for (int i = 0; i < int(bullets.size()); i++)
	{
		bullets[i].draw(window);
		std::cout << "Testing " << i << std::endl;
		bullets[i].fire();
	}
	for (int i = 0; i < int(bullets.size()); i++)
	{
		if (HitCheck(bullets[i]))
			bullets.erase(bullets.begin() + i);
	}
}

void Player::SetPosition(sf::Vector2f position) {
	body.setPosition(position);
}

bool Player::HitCheck(Bullet& bullet)
{
	if(GetCollider().CheckCollision(Collider(bullet.getBullet())))
	{
		health -= 50;
		return true;
	}
	return false;
}

bool Player::isUp(sf::RectangleShape &shape, float &baseHeight)
{
	return shape.getPosition().y <= baseHeight;
}

void Player::setName(const std::string& name)
{
	m_name = name;
}

void Player::setTimeout(sf::Time time)
{
	m_timeout = time;
}

void Player::setConnected(bool status)
{
	m_connected = status;
}

void Player::setPing(unsigned short ping)
{
	m_ping = ping;
}

unsigned short Player::getPing() {
	return m_ping;
}
sf::Vector2f Player::getPosition()
{
	return m_position;
}
sf::TcpSocket* Player::getSocket()
{
	return m_socket.get();
}

std::string Player::getName()
{
	return m_name;
}

int Player::getId()
{
	return m_id;
}

sf::Time Player::getTimeout()
{
	return m_timeout;
}
bool Player::isConnected()
{
	return m_connected;
}


