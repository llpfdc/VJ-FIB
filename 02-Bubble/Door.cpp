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
	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(DOOR_CLOSED, 8);
	sprite->addKeyframe(DOOR_CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DOOR_OPENING, 8);
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.f, 0.16f));
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.f, 0.33f));
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(DOOR_OPENING, glm::vec2(0.f, 0.66f));

	sprite->setAnimationSpeed(DOOR_OPENED, 8);
	sprite->addKeyframe(DOOR_OPENED, glm::vec2(0., 0.83f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));

}

void Door::update(int deltaTime) {
	sprite->update(deltaTime);
	if (opened) {
		if (sprite->animation() != DOOR_OPENING)
			sprite->changeAnimation(DOOR_OPENING);
	}
	else {
		sprite->changeAnimation(DOOR_CLOSED);
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