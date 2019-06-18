#include "Drawer.h"

using namespace std;
using namespace glm;

bool pause = false;

unsigned int framebuffer; //for fbo test only
unsigned int intermediateFBO; //for fbo test only
unsigned int texColorBuffer;
unsigned int rbo;

Texture* tForFbo;
Texture* tForIFbo;
VAO* msaaVAO;
Shader* msaaShader;


float screenVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};


Drawer::Drawer()
{
}


Drawer::~Drawer()
{
}

void drawerinit() {
	//init simone func
	initRand();

	//init camera
	cam = new Camera();

	//add spheres
	spheres.push_back(new Sphere(vec3(0,0,0), 1.0f, 720, 720));
	spheres[0]->bufferData(2);
	spheres[0]->createShader("vertex.txt", "frag.txt");

	//below is for multi sample fbo
	//FBO
	tForFbo = new Texture(window->width, window->height, true);
	globalTextures.push_back(tForFbo);
	framebuffer = tForFbo->framebuffer;
	//IFBO
	tForIFbo = new Texture(window->width, window->height);
	globalTextures.push_back(tForIFbo);
	intermediateFBO = tForIFbo->framebuffer;
	//VAO
	msaaVAO = new VAO(false);
	msaaVAO->bufferData(screenVertices, sizeof(screenVertices));
	msaaVAO->addAttribute(0, 2, 4 * sizeof(float), 0);
	msaaVAO->addAttribute(1, 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	//shader
	msaaShader = new Shader("msaaVertex.txt", "msaaFrag.txt");
	shaders.push_back(msaaShader);
	msaaShader->setInt(0, "screenTexture");
}

void logic() {
	updateDeltaTime();

	//update camera
	cam->update();

	//set up shader
	//shader 0
	mat4 model = mat4(1.0f);
	spheres[0]->update(model, cam->view, cam->projection);

	//reset size for framebuffer
	tForFbo->resetWidthHeight(window->width, window->height, true);
	tForIFbo->resetWidthHeight(window->width, window->height);
}

void draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	spheres[0]->draw(2);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, window->width, window->height, 0, 0, window->width, window->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	msaaVAO->use();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	msaaShader->use();
	glDisable(GL_DEPTH_TEST);
	tForIFbo->use(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
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
	for (int i = 0; i < spheres.size(); ++i) {
		free(spheres[i]);
	}

	free(cam);
}