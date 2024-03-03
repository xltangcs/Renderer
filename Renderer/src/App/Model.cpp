#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(const char* filename)
    : verts(), faces(), normals(), uvs(), modelName(filename)
{
    LoadModel(filename);
}

bool Model::LoadModel(const char* filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        printf("Failed to load model %s", filename);
        return false;
    }
    //printf("loading model %s", filename);
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            glm::vec3 v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            glm::vec3 n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            normals.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            glm::vec2 uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uvs.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<glm::vec3> f;
            glm::vec3 tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
    //std::cout << "# v# " << verts.size() << " f# " << faces.size() << " vt# " << uvs.size() << " vn# " << normals.size() << std::endl;
    //load_texture(filename, "_diffuse.tga", diffusemap_);
    //load_texture(filename, "_nm.tga", normalmap_);
    //load_texture(filename, "_spec.tga", specularmap_);
    return true;
}

std::vector<int> Model::GetFace(int index)
{
    std::vector<int> face;
    for (int i = 0; i < (int)faces[index].size(); i++)
        face.push_back(faces[index][i][0]);
    return face;
    
}

