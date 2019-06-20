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

Texture* hdrTex;
Shader* cubemapShader;

Shader* skyboxShader;
unsigned int captureFBO, captureRBO;
unsigned int envCubemap;

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

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void drawerinit() {
	//init simone func
	initRand();

	//init camera
	cam = new Camera();

	//add spheres
	Sphere* sph = new Sphere(vec3(0, 0, 0), 1.0f, 720, 720);
	sph->bufferData(2);
	sph->createShader("vertex.txt", "frag.txt");
	

	if (true) {
		sph->loadTexture("rustediron2_basecolor.png", 0, "albedoMap");
		sph->loadTexture("rustediron2_metallic.png", 1, "metallicMap");
		sph->loadTexture("rustediron2_roughness.png", 2, "roughnessMap");
		sph->loadTexture("rustediron2_normal.png", 3, "normalMap");
	}

	if (false) {
		sph->loadTexture("Iron-Scuffed_basecolor.png", 0, "albedoMap");
		sph->loadTexture("Iron-Scuffed_metallic.png", 1, "metallicMap");
		sph->loadTexture("Iron-Scuffed_roughness.png", 2, "roughnessMap");
		sph->loadTexture("Iron-Scuffed_normal.png", 3, "normalMap");
	}

	//pbr cubemap
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
	Texture::reverseImage();
	hdrTex = new Texture("Playa_Sunrise_8k.hdr", "", "", GL_RGB16F, 1);
	cubemapShader = new Shader("cubemapVertex.txt", "cubemapFrag.txt");
	cubemapShader->setInt(0, "equirectangularMap");
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	float width, height;
	width =  window->width;
	height = window->height;
	float cons = 3096.0f;
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cons, cons);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			cons, cons, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// convert HDR equirectangular environment map to cubemap equivalent
	cubemapShader->use();
	cubemapShader->setmat4(captureProjection, "projection");
	hdrTex->use(GL_TEXTURE0);

	glViewport(0, 0, cons, cons); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		cubemapShader->setmat4(captureViews[i], "view");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube(); // renders a 1x1 cube
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	skyboxShader = new Shader("skyboxVertex.txt", "skyboxFrag.txt");
	mat4 model = mat4(1.0f);
	glViewport(0, 0, width, height);
	

	//below is for multi sample fbo
	//FBO
	tForFbo = new Texture(window->width, window->height, true);
	framebuffer = tForFbo->framebuffer;
	//IFBO
	tForIFbo = new Texture(window->width, window->height);
	intermediateFBO = tForIFbo->framebuffer;
	//VAO
	msaaVAO = new VAO(false);
	msaaVAO->bufferData(screenVertices, sizeof(screenVertices));
	msaaVAO->addAttribute(0, 2, 4 * sizeof(float), 0);
	msaaVAO->addAttribute(1, 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	//shader
	msaaShader = new Shader("msaaVertex.txt", "msaaFrag.txt");
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

	//cubemap shader
	skyboxShader->setmat4(cam->view, "view");
	skyboxShader->setmat4(cam->projection, "projection");
}

void draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	spheres[0]->draw(2);
	//hdrTex->use(GL_TEXTURE0);
	//cubemapShader->use();
	skyboxShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	renderCube();

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

