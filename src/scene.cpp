#include "scene.h"

GameScene::GameScene(GameScene *p, GameModules *c) {
	modules = c;

	parent_scene = p;
	child_scene = nullptr;
	alive = true;
}

GameScene::~GameScene() {
	// Default black fill
	modules->display->set_fill(0x000000);
	modules->display->set_borderfill(0x000000);
}

void GameScene::kill() {
	// Allows engine to transition to next scene
	alive = false;
}

bool GameScene::get_alive() {
	return alive;
}

GameScene *GameScene::get_parent() {
	return parent_scene;
}

GameScene *GameScene::get_child() {
	return child_scene;
}

void GameScene::set_child(GameScene *next) {
	child_scene = next;
}

void GameScene::update() {
	// Override
	// Update modules of this scene
}

void GameScene::draw() {
	// Override
	// Draw scene sprites
}