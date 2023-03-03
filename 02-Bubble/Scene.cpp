#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 19

#define INIT_SKELETON_X_TILES 10
#define INIT_SKELETON_Y_TILES 4

Scene::Scene()
{
	map = NULL;
	player = NULL;
	door = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (door != NULL)
		delete door;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	initPlayer();
	initEnemies();
	initDoor();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::initPlayer() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
}
void Scene::initDoor() {
	door = new Door();
	door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	door->setPosition(glm::vec2((map->getDoorPos().x + 2)  * map->getTileSize(), map->getDoorPos().y * map->getTileSize()));
	door->setTileMap(map);
}

void Scene::initEnemies() {
	vector<glm::ivec2> positionEnemies = map->getEnemyPos();
	for (int i = 0; i < positionEnemies.size(); ++i) {
		Skeleton* skeleton = new Skeleton();
		skeleton->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

		skeleton->setPosition(glm::vec2(positionEnemies[i].x * map->getTileSize(), positionEnemies[i].y * map->getTileSize()));
		skeleton->setTileMap(map);
		skeletons.push_back(skeleton);
	}
}
void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int i = 0; i < skeletons.size(); i++) {
		skeletons[i]->update(deltaTime);
	}
	if (map->doorOpened()) {
		door->open();
	}
	door->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	for (int i = 0; i < skeletons.size(); i++) {
		skeletons[i]->render();
	}
	
	door->render();

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



