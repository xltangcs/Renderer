#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>


class Model
{
public:
	std::string modelName;
public:
	Model() = default;
	Model(const char* filename);
	~Model() = default;

	bool LoadModel(const char* filename);
	std::vector<int> GetFace(int index);

	int nVerts() { return (int)verts.size(); }
	int nFaces() { return (int)faces.size(); }

private:
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	std::vector<std::vector<glm::vec3> > faces; // attention, this Vec3i means vertex/uv/normal


};