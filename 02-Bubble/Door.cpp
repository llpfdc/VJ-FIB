#include "Door.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 8
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum DoorAnims {
	DOOR_CLOSED, DOOR_OPENING, DOOR_OPENED
};


void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/sprite_portal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 58), glm::vec2(0.125, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
	
	sprite->setAnimationSpeed(DOOR_CLOSED, 8);
	sprite->addKeyframe(DOOR_CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DOOR_OPENING, 2);
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.125f, 0.0f));
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(DOOR_OPENED, 2);
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0.375f, 0.0f));
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0.625f, 0.0f));
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0.75f, 0.0f));
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0.875f, 0.0f));
	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));

}

void Door::update(int deltaTime) {
	sprite->update(deltaTime);/*
	 if (opened && sprite->animation() == DOOR_CLOSED) {
		sprite->changeAnimation(DOOR_OPENING);
		timerDoor = deltaTime;
		
		cout << "open" << endl;
	 }
	 else  if (opened && sprite->animation() == DOOR_OPENING && deltaTime - timerDoor >= 1300 && timerDoor != -1) {
			sprite->changeAnimation(DOOR_OPENED);
			timerDoor = -1;
			cout << "opening" << endl;
	}
	else if(!opened) {
		sprite->changeAnimation(DOOR_CLOSED);
		cout << "closed" << endl;
	}*/
	
	if (opened && sprite->animation() == DOOR_CLOSED) {
		sprite->changeAnimation(DOOR_OPENING);
		cout << "open" << endl;
		timerDoor += deltaTime;
	}
	if (opened && sprite->animation() == DOOR_OPENING) {
		timerDoor += deltaTime;
	}
	if (opened && sprite->animation() == DOOR_OPENING && timerDoor >= 1000) {
		opening = false;
		sprite->changeAnimation(DOOR_OPENED);
	}
}

void Door::render() {
	sprite->render();
}

void Door::open() {
	opened = true;
}

void Door::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Door::setPosition(const glm::vec2& pos) {
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(posDoor.x), float(posDoor.y)));
}