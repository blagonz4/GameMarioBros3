#include "Scene.h"


Scene::Scene(float model)
{
	this->model = model;
}

void Scene::Render()
{
	int ani = -1;
	switch ((int)model) {
	case SCENE_STATE_1:
		ani = SCENE_ANI_1;
		break;
	case SCENE_STATE_2:
		ani = SCENE_ANI_2;
		break;
	case SCENE_STATE_3:
		ani = SCENE_ANI_3;
		break;
	case SCENE_STATE_4:
		ani = SCENE_ANI_4;
		break;
	case SCENE_STATE_5:
		ani = SCENE_ANI_5;
		break;
	case SCENE_STATE_6:
		ani = SCENE_ANI_6;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}
void Scene::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

Scene::~Scene()
{
}
