#include "Scene.h"

Scene::Scene(char* name)
    : sceneName(name)
{

}

int Scene::AddModel(const std::string& filename)
{
    int id = m_Models.size();
    std::shared_ptr<Model> model = std::make_shared<Model>();

    printf("Loading model %s\n", filename.c_str());
    if (model->LoadModel(filename.c_str()))
    {
        m_Models.emplace_back(model);

        for (int i = 0; i < model->nFaces(); i++)
        {
            std::vector<glm::vec3> facei = model->GetFace(i);
            Triangle t;
            for (int k = 0; k < 3; k++)
            {
                t.vert.push_back(model->GetVert(facei[k].x));
                t.normal.push_back(model->GetNormal(facei[k].z));
                t.uv.push_back(model->GetUV(facei[k].y));
            }
            m_Triangles.push_back(t);
        }

    }
    else
    {
        printf("Unable to load model %s\n", filename.c_str());
        return -1;
    }

    return id;
}

int Scene::AddGeometry(const BufferGeometry& geo)
{
    for (int index = 0; index <geo.indices.size(); index += 3)
    {
        Triangle t;
        for (int i = 0; i < 3; i++)
        {
            Vertex v = geo.vertices[geo.indices[index + i]];
            t.vert.push_back(v.Position);
            t.normal.push_back(v.Normal);
            t.uv.push_back(v.TexCoords);
        }
        m_Triangles.push_back(t);
    }

    return 0;
}

int Scene::AddLight(Light light)
{
    int id = m_Lights.size();
    m_Lights.emplace_back(light);
    return id;
}

void Scene::ProcessScene()
{
   /* for (auto model : m_Models)
    {
        for (int i = 0; i < model->nFaces(); i++)
        {
            std::vector<glm::vec3> facei = model->GetFace(i);
            Triangle t;
            for (int k = 0; k < 3; k++)
            {
                t.vert.push_back(model->GetVert(facei[k].x));
                t.normal.push_back(model->GetNormal(facei[k].z));
                t.uv.push_back(model->GetUV(facei[k].y));
            }
            m_Triangles.push_back(t);
        }
    }*/
}

