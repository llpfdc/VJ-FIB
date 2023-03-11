#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Skeleton
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
private:
	glm::ivec2 tileMapDispl, posSkeleton;
	int startY;
	bool right = true;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _SKELETON_INCLUDE
