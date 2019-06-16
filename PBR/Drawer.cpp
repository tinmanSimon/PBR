#include "Drawer.h"

using namespace std;
using namespace glm;

bool pause = false;

vector<glm::vec3> vegetation;
Drawer::Drawer()
{
}


Drawer::~Drawer()
{
}

void drawerinit() {
	//init camera
	cam = new Camera();
}

void logic() {


}

void draw() {
	
}

void cleanup() {
	for (int i = 0; i < VAOS.size(); ++i) {
		free(VAOS[i]);
	}
	for (int i = 0; i < shaders.size(); ++i) {
		free(shaders[i]);
	}
	for (int i = 0; i < Models.size(); ++i) {
		free(Models[i]);
	}
	for (int i = 0; i < globalTextures.size(); ++i) {
		free(globalTextures[i]);
	}
	free(cam);
}