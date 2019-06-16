#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) : vao{ new VAO(true) }
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(Shader* shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		shader->setInt(i, (name + number).c_str());
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	vao->use();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	vao->bufferData(&vertices[0], vertices.size() * sizeof(Vertex), &indices[0], indices.size() * sizeof(unsigned int));
	vao->addAttribute(0, 3, sizeof(Vertex), (void*)0);
	vao->addAttribute(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	vao->addAttribute(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	vao->addAttribute(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	vao->addAttribute(4, 3, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	glBindVertexArray(0);
}
