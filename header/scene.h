#ifndef SCENE_H
#define SCENE_H

#include "object.h"

class Scene
{
	const char *sceneName;

	int visible;

	static const int maxNumberOfObjects = 100;
	int currentNumberOfObjects = 0;
	int currentObj = 1;

	Object objects[maxNumberOfObjects];

public:
	Scene(const char *sceneName = "New scene");
	Scene(Scene &sc);

	GLboolean canMoveObject(Object *obj);

	int addObject(Object &newObject);
	int removeObject(const char *name);
	Object& getObject(int ind);
	Object& getCurrObject();
	int getNumberObjects();

	void hide();
	void show();
	int isVisible();

	~Scene();
};

#endif