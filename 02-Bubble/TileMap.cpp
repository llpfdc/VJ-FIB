#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);
	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	ShProgram = program;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	spritesheetLlave.loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteLlave = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheetLlave, &ShProgram);
	spriteLlave->setNumberAnimations(1);
	spriteLlave->setAnimationSpeed(0, 8);
	spriteLlave->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	spriteLlave->changeAnimation(0);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

void TileMap::render()
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
	if (firstSprite) {
		for (int i = 0; i < positions.size(); i++) {
			spriteSuelo->setPosition(positions[i]);
			spriteSuelo->render();
		}
	}
	if (positions.size() >= casillasNivel && showKey) {
		spriteLlave->setPosition(glm::ivec2((keyCoords.x + 2) * tileSize, (keyCoords.y + 1) * tileSize));
		spriteLlave->render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	////////////////
	getline(fin, line);
	sstream.str(line);
	sstream >> casillasNivel;
	//////////////////

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile == 5) {
				keyCoords.x = i;
				keyCoords.y = j;
			}
			if (tile == 6) {
				glm::ivec2 pos = glm::vec2(i, j);
				positionsEnemies.push_back(pos);
			}
			if (tile == 7) {
				doorCoords.x = i;
				doorCoords.y = j;
			}
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}

		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


bool TileMap::collisionMoveEnemy(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool& right) const {

	if (right) {
		int x, y0, y1;
		x = (pos.x + size.x - 1) / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++) {
			if (map[(y + 1) * mapSize.x + x] == 2 || map[(y + 1) * mapSize.x + x] == 1)
				return false;
		}

	}
	else {
		int x, y0, y1;

		x = pos.x / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++) {
			if (map[(y + 1) * mapSize.x + x] == 2 || map[(y + 1) * mapSize.x + x] == 1)
				return false;
		}
	}
	return true;
}
bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, bool isEnemy)
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 7 && !isEnemy && doorOpen) {
			cout << "you win" << endl;
		}
		if (map[y * mapSize.x + x] == 5 && !isEnemy) {
			showKey = false;
			doorOpen = true;
		}
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] < 5)
			return true;
	}

	return false;
}
bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, bool isEnemy)
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 7 && !isEnemy && doorOpen) {
			cout << "you win" << endl;
		}
		if (map[y * mapSize.x + x] == 5 && !isEnemy) {
			showKey = false;
			doorOpen = true;
		}
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] < 5)
			return true;
	}

	return false;
}
bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool isEnemy)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 7 && !isEnemy && doorOpen) {
			cout << "you win" << endl;
		}
		if (map[y * mapSize.x + x] == 5 && !isEnemy) {
			showKey = false;
			doorOpen = true;
		}
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] < 5)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}
void TileMap::colisionGroundTileLeft(const glm::ivec2& pos, const glm::ivec2& size)
{

	int x0, x1, y;

	x1 = pos.x / tileSize;
	x0 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize + 1;
	for (int x = x0; x >= x1; x--)
	{
		if (map[y * mapSize.x + x] == 2)
		{
			map[y * mapSize.x + x] = 1;
			if (!firstSprite) {
				spritesheetSuelo.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
				spriteSuelo = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheetSuelo, &ShProgram);
				spriteSuelo->setNumberAnimations(1);
				spriteSuelo->setAnimationSpeed(0, 8);
				spriteSuelo->addKeyframe(0, glm::vec2(0.5f, 0.5f));
				spriteSuelo->changeAnimation(0);
			}
			positions.push_back(glm::ivec2((x + 2) * tileSize, (y + 1) * tileSize));
			firstSprite = true;
		}
	}

}
void TileMap::colisionGroundTileRight(const glm::ivec2& pos, const glm::ivec2& size)
{

	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize + 1;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 2)
		{
			map[y * mapSize.x + x] = 1;
			if (!firstSprite) {
				spritesheetSuelo.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
				spriteSuelo = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheetSuelo, &ShProgram);
				spriteSuelo->setNumberAnimations(1);
				spriteSuelo->setAnimationSpeed(0, 8);
				spriteSuelo->addKeyframe(0, glm::vec2(0.5f, 0.5f));
				spriteSuelo->changeAnimation(0);
			}
			positions.push_back(glm::ivec2((x + 2) * tileSize, (y + 1) * tileSize));
			firstSprite = true;
		}
	}
}

vector<glm::ivec2> TileMap::getEnemyPos() {
	return positionsEnemies;
}
glm::ivec2 TileMap::getDoorPos()
{
	return doorCoords;
}
bool TileMap::doorOpened() {
	return doorOpen;
}

bool TileMap::collisionMoveLeftHit(const glm::ivec2& pos, const glm::ivec2& size)
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++) {
		for (int i = 0; i < positionsEnemies.size(); ++i) {
			if (positionsEnemies[i].x + size.x > pos.x / tileSize && (pos.x + size.x)/tileSize > positionsEnemies[i].x) {
				for (int yEnemy = positionsEnemies[i].y / tileSize; yEnemy < (positionsEnemies[i].y + size.y - 1) / tileSize; yEnemy++) {
					if (yEnemy == i) {
						//cout << "hit" << endl;
					}
				}
			}
		}
	}
	return false;
}