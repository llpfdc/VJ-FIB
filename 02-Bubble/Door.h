#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE
#include "Sprite.h"
#include "TileMap.h"


class Door
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void open();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, posDoor;
	bool opened = false;
	bool opening = false;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int timerDoor = -1;
	

};
#endif
