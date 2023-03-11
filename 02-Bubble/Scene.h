#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Skeleton.h"
#include "Door.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void initPlayer();
	void initEnemies();
	bool collide(glm::ivec2 positionSkeleton);
	void initDoor();
private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	vector<Skeleton*> skeletons;
	Door* door;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool alreadyOpen = false;
};


#endif // _SCENE_INCLUDE