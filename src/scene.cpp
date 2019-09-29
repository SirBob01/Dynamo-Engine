#include "scene.h"

Dynamo::Scene::Scene(Dynamo::Scene *parent, Dynamo::Modules *m) {
	modules = m;
	
	parent_scene = parent;
	child_scene = nullptr;
	alive = true;
}

Dynamo::Scene::~Scene() {
	// Default black fill
	modules->display->set_fill(0x000000);
	modules->display->set_borderfill(0x000000);
}

void Dynamo::Scene::kill() {
	// Allows engine to transition to next scene
	alive = false;
}

bool Dynamo::Scene::get_alive() {
	return alive;
}

Dynamo::Scene *Dynamo::Scene::get_parent() {
	return parent_scene;
}

Dynamo::Scene *Dynamo::Scene::get_child() {
	return child_scene;
}

void Dynamo::Scene::set_child(Scene *next) {
	child_scene = next;
}

void Dynamo::Scene::update() {
	// Override
	// Update modules of this scene
}

void Dynamo::Scene::draw() {
	// Override
	// Draw scene sprites
}