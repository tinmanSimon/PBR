#ifndef DRAWER_H
#define DRAWER_H

#include "Camera.h"
#include "VAO.h"
#include "Shader.h"
#include "Sphere.h"

extern float vertices[];
extern bool pause;
extern int skyboxes;

/*
This is the class that deals with logic and drawings
*/
class Drawer
{
public:
	Drawer();
	~Drawer();
};

void drawerinit();

void logic();

void draw();

void cleanup();

#endif