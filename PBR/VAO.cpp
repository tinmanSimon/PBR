#include "VAO.h"

using namespace std;
vector<VAO*> VAOS;

VAO::VAO(bool weNeedEBO) : ebo{ 0 }
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	if (weNeedEBO) glGenBuffers(1, &ebo);
	VAOS.push_back(this);
}


VAO::~VAO()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void VAO::bufferData(void* data, GLsizeiptr size, void* ebodata, GLsizeiptr ebosize, GLenum usage) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	//if (ebosize > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebosize, ebodata, GL_STATIC_DRAW);
	//}
	
}

void VAO::addAttribute(GLuint index,
	GLint size,
	GLsizei stride,
	const GLvoid* pointer,
	GLenum type,
	GLboolean normalized) {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}

void VAO::use() {
	glBindVertexArray(vao);
}

