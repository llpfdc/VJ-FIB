#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 8
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum SkeletonAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Skeleton::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y)));

}

void Skeleton::update(int deltaTime) {
	sprite->update(deltaTime);
	if (!right) {
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posSkeleton.x -= 2;
		if (map->collisionMoveLeft(posSkeleton, glm::ivec2(32, 32),true)) {
			posSkeleton.x += 2;
			sprite->changeAnimation(STAND_LEFT);
			right = true;
		}
		if (map->collisionMoveEnemy(posSkeleton, glm::ivec2(32, 32), &posSkeleton.y, right)) {
			posSkeleton.x += 2;
			sprite->changeAnimation(STAND_LEFT);
			right = true;
		}
	}
	else {  // if (right)
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posSkeleton.x += 2;
		if (map->collisionMoveRight(posSkeleton, glm::ivec2(32, 32), true)) {
			posSkeleton.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
			right = false;
		}
		if (map->collisionMoveEnemy(posSkeleton, glm::ivec2(32, 32), &posSkeleton.y, right)) {
			posSkeleton.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
			right = false;
		}
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y)));
}

void Skeleton::render() {
	sprite->render();
}

void Skeleton::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Skeleton::setPosition(const glm::vec2& pos) {
	posSkeleton = pos;
	sprite->setPosition(glm::vec2(float(posSkeleton.x), float(posSkeleton.y)));
}
