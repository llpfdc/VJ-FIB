#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool alive();
	bool invulnerable();
	void hit();
	glm::ivec2 getPosition();
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Texture textu;
	Sprite *sprite;
	int lifes = 3;
	bool invulnerability = false;
	int invulnerabilityTime = 0;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


