#include "scene.h"

namespace Dynamo {
	Scene::Scene(Scene *parent, Modules *m) {
		modules = m;
		
		parent_scene = parent;
		child_scene = nullptr;
		alive = true;
	}

	Scene::~Scene() {
		// Default black fill
		modules->display->set_fill(0x000000);
		modules->display->set_borderfill(0x000000);
	}

	void Scene::kill() {
		// Allows engine to transition to next scene
		alive = false;
	}

	bool Scene::get_alive() {
		return alive;
	}

	Scene *Scene::get_parent() {
		return parent_scene;
	}

	Scene *Scene::get_child() {
		return child_scene;
	}

	void Scene::set_child(Scene *next) {
		child_scene = next;
	}

	void Scene::update() {
		// Override
		// Update modules of this scene
	}

	void Scene::draw() {
		// Override
		// Draw scene sprites
	}
}