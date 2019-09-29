#include "scene.h"

namespace Dynamo {
	Scene::Scene(Scene *parent, Modules *modules) {
		modules_ = modules;
		
		parent_ = parent;
		child_ = nullptr;
		alive_ = true;
	}

	Scene::~Scene() {
		// Default black fill
		modules_->display->set_fill({0, 0, 0});
		modules_->display->set_borderfill({0, 0, 0});
	}

	void Scene::kill() {
		// Allows engine to transition to next scene
		alive_ = false;
	}

	bool Scene::get_alive() {
		return alive_;
	}

	Scene *Scene::get_parent() {
		return parent_;
	}

	Scene *Scene::get_child() {
		return child_;
	}

	void Scene::set_child(Scene *next) {
		child_ = next;
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