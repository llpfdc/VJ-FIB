#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() ;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool isEnemy) ;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool isEnemy) ;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool isEnemy);
	void colisionGroundTileLeft(const glm::ivec2& pos, const glm::ivec2& size) ;
	void colisionGroundTileRight(const glm::ivec2& pos, const glm::ivec2& size);
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	bool collisionMoveEnemy(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool& right) const;
	vector<glm::ivec2> getEnemyPos();
	glm::ivec2 getDoorPos();
	bool doorOpened();
private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	ShaderProgram ShProgram; 
	Sprite* sprite;
	bool firstSprite = false;
	Texture spritesheetSuelo;
	Sprite* spriteSuelo;
	Texture spritesheetLlave;
	Sprite* spriteLlave;
	vector<glm::ivec2> positions;
	int casillasNivel;
	glm::ivec2 keyCoords;
	glm::ivec2 doorCoords;
	vector<glm::ivec2> positionsEnemies;
	bool showKey = true;
};


#endif // _TILE_MAP_INCLUDE


