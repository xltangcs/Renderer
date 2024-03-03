#include "Scene.h"

int Scene::AddModel(const std::string& filename)
{
    int id = m_Models.size();
    std::shared_ptr<Model> model = std::make_shared<Model>();

    printf("Loading model %s\n", filename.c_str());
    if (model->LoadModel(filename.c_str()))
    {
        m_Models.emplace_back(model);
    }
    else
    {
        printf("Unable to load model %s\n", filename.c_str());
        return -1;
    }

    return id;
}

