#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
#define JUMP_HEIGHT 55
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;



	spritesheet.loadFromFile("images/main.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.125f));

	sprite->setAnimationSpeed(STAND_RIGHT, 4);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.125f));


	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.125f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.375f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.375f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));


	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.125f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.375f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.375f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (invulnerability) ++invulnerabilityTime;
	if (invulnerabilityTime >= 80) {
		invulnerability = 0;
		invulnerabilityTime = 0;
	}
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{

		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		map->colisionGroundTileLeft(posPlayer, glm::ivec2(32, 32));
		
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32),false))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
		
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		map->colisionGroundTileRight(posPlayer, glm::ivec2(32, 32));
		
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32),false))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
		
	}
	else
	{
		if (!bJumping && sprite->animation() == STAND_RIGHT){
			map->colisionGroundTileRight(posPlayer, glm::ivec2(32, 32));
			
		}
		else if (!bJumping && sprite->animation() == STAND_LEFT) {
			map->colisionGroundTileLeft(posPlayer, glm::ivec2(32, 32));
	
		}if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y,false);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y,false))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

bool Player::alive() {
	return (lifes > 0);
}

bool Player::invulnerable() {
	return invulnerability;
}
void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::hit()
{
	--lifes;
	invulnerability = true;
	invulnerabilityTime = 0;
}
glm::ivec2 Player::getPosition() {
	return posPlayer;
}

